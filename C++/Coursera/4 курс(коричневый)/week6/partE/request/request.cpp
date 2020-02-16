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

			Settings SettingsRead(const map<string, Node>& settings_dict) {
				Settings settings;
				settings.bus_wait_time = static_cast<uint16_t>(settings_dict.at("bus_wait_time").AsDouble());
				settings.bus_velocity = settings_dict.at("bus_velocity").AsDouble();
				return settings;
			}
		}

		pair<Settings, Splited> Read(istream& in_stream) {
			Document document = Load(in_stream);
			pair<Settings, Splited> result;
			const auto& dict = document.GetRoot().AsMap();
			if (auto it = dict.find("routing_settings"); it != dict.end()) {
				result.first = SettingsRead(it->second.AsMap());
			}
			if (auto it = dict.find("base_requests"); it != dict.end()) {
				result.second.modify = PartionRead(Mode::MODIFY, it->second.AsArray());
			}
			if (auto it = dict.find("stat_requests"); it != dict.end()) {
				result.second.read = PartionRead(Mode::READ, it->second.AsArray());
			}
			return result;
		}

		Document Process(const pair<Settings, Splited>& input_data) {
			vector<Node> responses;
			const auto& [settings, requests] = input_data;
			responses.reserve(requests.read.size());

			BusManager manager;
			for (const auto& request_holder : requests.modify) {
				const auto& request = static_cast<const IModify&>(*request_holder);
				request.Process(manager);
			}

			BusRouter router(manager, settings.bus_velocity, settings.bus_wait_time);
			for (const auto& request_holder : requests.read) {
				if (request_holder->getType() == Type::ROUTE_INFO) {
					responses.emplace_back(static_cast<const IRead<Node, BusRouter>&>(*request_holder).Process(router));
				}
				else {
					responses.emplace_back(static_cast<const IRead<Node, BusManager>&>(*request_holder).Process(manager));
				}
			}

			return Document(Node(move(responses)));
		}
	}

	// IRequest definition
	IRequest::IRequest(Type t) : type(t) {}

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
		case Type::ROUTE_INFO:
			return make_unique<RouteInfo>();
		default:
			return nullptr;
		}
	}

	Type IRequest::getType() const {
		return type;
	}

	IModify::IModify(Type t) : IRequest(t) {}

	template <typename ResultType, typename ReadType>
	IRead<ResultType, ReadType>::IRead(Type t) : IRequest(t) {}

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
	AddBus::AddBus() : IModify(Type::ADD_BUS), is_roundtrip(false) {}

	void AddBus::ParseFrom(const Node& node) {
		const auto& dict = node.AsMap();
		bus_name = dict.at("name").AsString();

		const vector<Node>& node_stops = dict.at("stops").AsArray();
		is_roundtrip = dict.at("is_roundtrip").AsBool();
		stops.reserve(node_stops.size());

		for (const auto& stop : node_stops) {
			stops.emplace_back(stop.AsString());
		}
	}

	void AddBus::Process(BusManager& manager) const {
		manager.AddBus(bus_name, stops, is_roundtrip);
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
			node["route_length"] = bus_info->road_info.distances.back();
			node["curvature"] = bus_info->road_info.curvature;
			node["stop_count"] = static_cast<double>(bus_info->stops.size());
			node["unique_stop_count"] = static_cast<double>(bus_info->unique_amount);
		}
		return Node(move(node));
	}

	//RouteInfo definition
	RouteInfo::RouteInfo() : IRead(Type::ROUTE_INFO), id(0) {}

	void RouteInfo::ParseFrom(const Node& node) {
		const auto& dict = node.AsMap();
		from_stop = dict.at("from").AsString();
		to_stop = dict.at("to").AsString();
		id = static_cast<size_t>(dict.at("id").AsDouble());
	}

	Node RouteInfo::Process(const BusRouter& router) const {
		auto route_info = router.RouteInfo(from_stop, to_stop);

		map<string, Node> node;
		node["request_id"] = Node(static_cast<double>(id));
		if (!route_info) {
			node["error_message"] = Node(string("not found"));
		}
		else {
			auto& route_data = route_info.value();
			node["total_time"] = Node(route_data.total_time);

			vector<Node> items;
			items.reserve(route_data.route.size());
			for (auto& trip : route_data.route) {
				map<string, Node> item;

				item["stop_name"] = Node(move(trip.from_stop_name));
				item["time"] = Node(static_cast<double>(trip.wait_time));
				item["type"] = Node(string("Wait"));
				items.emplace_back(move(item));

				item["bus"] = Node(move(trip.bus_name));
				item["span_count"] = Node(static_cast<double>(trip.span_count));
				item["time"] = Node(static_cast<double>(trip.bus_time));
				item["type"] = Node(string("Bus"));
				items.emplace_back(move(item));
			}
			node["items"] = Node(move(items));
		}
		return Node(move(node));
	}
}
