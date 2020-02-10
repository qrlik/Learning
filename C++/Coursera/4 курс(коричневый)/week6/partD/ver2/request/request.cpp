#include "request.h"
#include <algorithm>
#include <iterator>

using namespace std;

namespace Request {
	optional<Type> ConvertTypeFromString(const Mode mode, string_view str) {
		const auto& STR_TO_TYPE = (mode == Mode::MODIFY) ? STR_TO_MODIFY_TYPE : STR_TO_READ_TYPE;

		if (const auto it = STR_TO_TYPE.find(str);
			it != STR_TO_TYPE.end()) {
			return it->second;
		}
		else {
			return nullopt;
		}
	}

	namespace Handler {
		namespace {
			vector<RequestHolder> PartionRead(Mode mode, const vector<Node>& nodes) {
				vector<RequestHolder> requests;
				requests.reserve(nodes.size());
				for (const auto& node : nodes) {
					if (auto request = IRequest::Parse(mode, node)) {
						requests.emplace_back(move(request));
					}
				}
				if (mode == Mode::MODIFY) {
					partition(requests.begin(), requests.end(), [](const RequestHolder& r_h)
						{ return r_h->getType() == Type::ADD_STOP; });
				}
				return requests;
			}
		}

		Splited Read(istream& in_stream) {
			Document document = Load(in_stream);
			Splited result;
			const auto& dict = document.GetRoot().AsMap();
			if (auto it = dict.find("base_requests"); it != dict.end()) {
				result.modify = PartionRead(Mode::MODIFY, it->second.AsArray());
			}
			if (auto it = dict.find("stat_requests"); it != dict.end()) {
				result.read = PartionRead(Mode::READ, it->second.AsArray());
			}
			return result;
		}

		Document Process(const Splited& input_data) {
			vector<Node> responses;
			responses.reserve(input_data.read.size());

			BusManager manager;

			for (const auto& request_holder : input_data.modify) {
				const auto& request = static_cast<const IModify&>(*request_holder);
				request.Process(manager);
			}

			for (const auto& request_holder : input_data.read) {
				const auto& request = static_cast<const IRead<Node>&>(*request_holder);
				responses.emplace_back(request.Process(manager));
			}

			return Document(Node(responses));
		}
	}

	// IRequest definition
	RequestHolder IRequest::Parse(const Mode mode, const Node& node) {
		const auto request_type = ConvertTypeFromString(mode, node.AsMap().at("type").AsString());
		if (!request_type) {
			return nullptr;
		}
		RequestHolder request = IRequest::Create(*request_type);
		if (request) {
			request->ParseFrom(node);
		}
		return request;
	}

	RequestHolder IRequest::Create(Type type) {
		switch (type) {
		case Type::ADD_STOP:
			return make_unique<AddStop>();
		case Type::ADD_BUS:
			return make_unique<AddBus>();
		case Type::STOP_INFO:
			return make_unique<StopInfo>();
		case Type::BUS_INFO:
			return make_unique<BusInfo>();
		default:
			return nullptr;
		}
	}

	Type IRequest::getType() const {
		return type;
	}

	// AddStop definition
	AddStop::AddStop() : IModify(Type::ADD_STOP), latitude(0), longitude(0) {}

	void AddStop::ParseFrom(const Node& node) {
		const auto& dict = node.AsMap();
		stop_name = dict.at("name").AsString();
		latitude = dict.at("latitude").AsDouble();
		longitude = dict.at("longitude").AsDouble();

		for (const auto& [stop, distance] : dict.at("road_distances").AsMap()) {
			distances[stop] = static_cast<size_t>(distance.AsDouble());
		}
	}

	void AddStop::Process(BusManager& manager) const {
		manager.AddStop(stop_name, distances, latitude, longitude);
	}

	// AddBus definition
	AddBus::AddBus() : IModify(Type::ADD_BUS) {}

	void AddBus::ParseFrom(const Node& node) {
		const auto& dict = node.AsMap();
		bus_name = dict.at("name").AsString();

		const vector<Node>& node_stops = dict.at("stops").AsArray();
		bool is_roundtrip = dict.at("is_roundtrip").AsBool();
		stops.reserve(is_roundtrip ? node_stops.size() : node_stops.size() * 2 - 1);

		for (const auto& stop : node_stops) {
			stops.push_back(stop.AsString());
		}
		if (!is_roundtrip) {
			stops.insert(stops.end(), stops.rbegin() + 1, stops.rend()); // no reallocate, iterators OK
		}
	}

	void AddBus::Process(BusManager& manager) const {
		manager.AddBus(bus_name, stops);
	}

	// StopInfo definition
	StopInfo::StopInfo() : IRead(Type::STOP_INFO), id(0) {}

	void StopInfo::ParseFrom(const Node& node) {
		const auto& dict = node.AsMap();
		stop_name = dict.at("name").AsString();
		id = static_cast<size_t>(dict.at("id").AsDouble());
	}
	
	Node StopInfo::Process(const BusManager& manager) const {
		auto stop_info = manager.StopInfo(stop_name);
		map<string, Node> node;
		node["request_id"] = Node(static_cast<double>(id));
		if (!stop_info) {
			node["error_message"] = Node(string("not found"));
		}
		else {
			const auto& buses_set = stop_info->buses;
			vector<Node> buses;
			buses.reserve(buses_set.size());
			transform(buses_set.begin(), buses_set.end(), back_inserter(buses),
				[](string_view bus) {
					return Node(string(bus));
				});
			node["buses"] = move(buses);
		}
		return Node(move(node));
	}

	// BusInfo definition
	BusInfo::BusInfo() : IRead(Type::BUS_INFO), id(0) {}

	void BusInfo::ParseFrom(const Node& node) {
		const auto& dict = node.AsMap();
		bus_name = dict.at("name").AsString();
		id = static_cast<size_t>(dict.at("id").AsDouble());
	}

	Node BusInfo::Process(const BusManager& manager) const {
		auto bus_info = manager.BusInfo(bus_name);

		map<string, Node> node;
		node["request_id"] = Node(static_cast<double>(id));
		if (!bus_info) {
			node["error_message"] = Node(string("not found"));
		}
		else {
			node["route_length"] = bus_info->length.road_length;
			node["curvature"] = bus_info->length.curvature;
			node["stop_count"] = static_cast<double>(bus_info->stops.size());
			node["unique_stop_count"] = static_cast<double>(bus_info->unique_stops);
		}
		return Node(move(node));
	}
}
