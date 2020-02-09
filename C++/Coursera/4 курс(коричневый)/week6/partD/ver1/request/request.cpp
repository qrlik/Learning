#include "request.h"
#include <algorithm>
#include <iterator>

using namespace std;

namespace Request {
	Format GetFormat(int symb) {
		if (symb == '{') {
			return Format::JSON;
		}
		return Format::STANDARD;
	}

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
			vector<RequestHolder> PartionRead(Mode mode, istream& in_stream) {
				const int count = ReadNumberOnLine<int>(in_stream);

				vector<RequestHolder> requests;
				if (count <= 0) {
					return requests;
				}
				requests.reserve(count);
				string request_str;

				for (int i = 0; i < count && getline(in_stream, request_str); ++i) {
					if (auto request = IRequest::Parse(mode, string_view(request_str))) {
						requests.emplace_back(move(request));
					}
				}
				if (mode == Mode::MODIFY) {
					partition(requests.begin(), requests.end(), [](const RequestHolder& r_h)
						{ return r_h->getType() == Type::ADD_STOP; });
				}
				return requests;
			}

			vector<RequestHolder> PartionRead(Mode mode, const vector<Json::Node>& nodes) {
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

			Splited JsonRead(const Json::Document& document) {
				using namespace Json;
				Splited result;
				const map<string, Node>& dict = document.GetRoot().AsMap();
				if (auto it = dict.find("base_requests"); it != dict.end()) {
					result.modify = PartionRead(Mode::MODIFY, it->second.AsArray());
				}
				if (auto it = dict.find("stat_requests"); it != dict.end()) {
					result.read = PartionRead(Mode::READ, it->second.AsArray());
				}
				return result;
			}

			auto GetProcessFunc(Format format) {
				string (IRead<string>::*func)(const BusManager&) const;
				switch (format)
				{
				case Request::Format::JSON:
					func = &IRead<string>::ProcessJson;
					break;
				default:
					func = &IRead<string>::Process;
					break;
				}
				return func;
			}
		}

		pair<Splited, Format> Read(istream& in_stream) {
			Format format = GetFormat(in_stream.peek());
			switch (format) {
			case Format::JSON:
				return { JsonRead(Json::Load(in_stream)), format };
			default:
				return { Splited{ PartionRead(Mode::MODIFY, in_stream),
					PartionRead(Mode::READ, in_stream) }, format };
			}
		}

		vector<string> Process(const pair<Splited, Format>& input_data) {
			vector<string> responses;
			responses.reserve((input_data.second == Format::JSON) ?
				input_data.first.read.size() + 2 : input_data.first.read.size());

			BusManager manager;
			auto process_func = GetProcessFunc(input_data.second);

			for (const auto& request_holder : input_data.first.modify) {
				const auto& request = static_cast<const IModify&>(*request_holder);
				request.Process(manager);
			}

			if (input_data.second == Format::JSON) {
				responses.push_back("[");
			}
			for (const auto& request_holder : input_data.first.read) {
				const auto& request = static_cast<const IRead<string>&>(*request_holder);
				responses.push_back((request.*process_func)(manager));
			}
			if (input_data.second == Format::JSON) {
				responses.back().pop_back();
				responses.push_back("]");
			}

			return responses;
		}

		void Print(const vector<string>& responses, ostream& os_stream) {
			bool first = true;
			for (const auto& response : responses) {
				if (!first) {
					os_stream << '\n';
				}
				else {
					first = false;
				}
				os_stream << response;
			}
		}
	}

	// IRequest definition
	RequestHolder IRequest::Parse(const Mode mode, string_view str) {
		const auto request_type = ConvertTypeFromString(mode, ReadToken(str));
		if (!request_type) {
			return nullptr;
		}
		RequestHolder request = IRequest::Create(*request_type);
		if (request) {
			request->ParseFrom(str);
		}
		return request;
	}

	RequestHolder IRequest::Parse(const Mode mode, const Json::Node& node) {
		const string& type_str = node.AsMap().at("type").AsString();
		const auto request_type = ConvertTypeFromString(mode, type_str);
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

	void AddStop::ParseFrom(string_view input) {
		stop_name = string(ReadToken(input, ": "));
		latitude = ConvertToDouble(ReadToken(input, ", "));
		longitude = ConvertToDouble(ReadToken(input, ", "));

		while (!input.empty()) { /* Stop X: latitude, longitude, D1m to stop1, D2m to stop2, ... */
			int length = ConvertToInt(ReadToken(input, "m to "));
			distances[string(ReadToken(input, ", "))] = length;
		}
	}

	void AddStop::ParseFrom(const Json::Node& node) {
		const map<string, Json::Node>& dict = node.AsMap();
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

	void AddBus::ParseFrom(string_view input) {
		bus_name = string(ReadToken(input, ": "));

		string delimiter = (input.find(" - ") != input.npos) ? " - " : " > ";

		while (!input.empty()) {
			stops.push_back(string(ReadToken(input, delimiter)));
		}

		if (delimiter == " - " && !stops.empty()) { /* зацикливаем маршрут */
			vector<string> tmp_stops;
			tmp_stops.reserve(stops.size() * 2 - 1);
			tmp_stops.insert(tmp_stops.end(), stops.begin(), stops.end());
			move(stops.rbegin() + 1, stops.rend(), back_inserter(tmp_stops));
			stops = move(tmp_stops);
		}
	}

	void AddBus::ParseFrom(const Json::Node& node) {
		using namespace Json;
		const map<string, Node>& dict = node.AsMap();
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

	void StopInfo::ParseFrom(string_view input) {
		stop_name = string(input);
	}

	void StopInfo::ParseFrom(const Json::Node& node) {
		const map<string, Json::Node>& dict = node.AsMap();
		stop_name = dict.at("name").AsString();
		id = static_cast<size_t>(dict.at("id").AsDouble());
	}
	
	string StopInfo::Process(const BusManager& manager) const {
		auto stop_info = manager.StopInfo(stop_name);
		ostringstream result;
		result << "Stop " << stop_name << ": ";
		if (!stop_info) {
			result << "not found";
		}
		else if (const auto& buses = stop_info->buses; buses.empty()) {
			result << "no buses";
		}
		else {
			result << "buses";
			for (string_view bus : buses) {
				result << " " << bus;
			}
		}
		return result.str();
	}

	string StopInfo::ProcessJson(const BusManager& manager) const {
		auto stop_info = manager.StopInfo(stop_name);
		ostringstream result;
		result << "{\n" << "\"request_id\": " << id << ",\n";
		if (!stop_info) {
			result << "\"error_message\": \"not found\"";
		}
		else {
			result << "\"buses\": [";
			bool first = true;
			for (string_view bus : stop_info->buses) {
				if (!first) {
					result << ",";
				}
				else {
					first = false;
				}
				result << "\n\"" << bus << "\"";
			}
			if (!stop_info->buses.empty()) {
				result << "\n";
			}
			result << "]";
		}
		result << "\n},";
		return result.str();
	}

	// BusInfo definition
	BusInfo::BusInfo() : IRead(Type::BUS_INFO), id(0) {}

	void BusInfo::ParseFrom(string_view input) {
		bus_name = string(input);
	}

	void BusInfo::ParseFrom(const Json::Node& node) {
		const map<string, Json::Node>& dict = node.AsMap();
		bus_name = dict.at("name").AsString();
		id = static_cast<size_t>(dict.at("id").AsDouble());
	}

	string BusInfo::Process(const BusManager& manager) const {
		auto bus_info = manager.BusInfo(bus_name);
		ostringstream result;
		result.setf(ios::fixed);
		result.precision(6);
		result << "Bus " << bus_name << ": ";
		if (!bus_info) {
			result << "not found";
		}
		else {
			result << bus_info->stops.size() << " stops on route, " <<
				bus_info->unique_stops << " unique stops, " <<
				bus_info->length.road_length << " route length, " <<
				bus_info->length.curvature << " curvature";
		}
		return result.str();
	}

	string BusInfo::ProcessJson(const BusManager& manager) const {
		auto bus_info = manager.BusInfo(bus_name);
		ostringstream result;
		result.setf(ios::fixed);
		result.precision(6);
		result << "{\n" << "\"request_id\": " << id << ",\n";
		if (!bus_info) {
			result << "\"error_message\": \"not found\"";
		}
		else {
			result << "\"route_length\": " << bus_info->length.road_length << ",\n" <<
				"\"curvature\": " << bus_info->length.curvature << ",\n" <<
				"\"stop_count\": " << bus_info->stops.size() << ",\n" <<
				"\"unique_stop_count\": " << bus_info->unique_stops;
		}
		result << "\n},";
		return result.str();
	}
}
