#include "request.h"
#include <algorithm>
#include <iterator>

using namespace std;

namespace Request {
	namespace {
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
					if (auto request = IRequest::Parse(mode, request_str)) {
						requests.push_back(move(request));
					}
				}

				if (mode == Mode::MODIFY) { /* т.к запросы могут идти в перемешку, а для рассчета length нам нужны все остановки, разделяем запросы */
					partition(requests.begin(), requests.end(), [](const RequestHolder& r_h)
						{ return r_h->getType() == Type::ADD_STOP; });
				}

				return requests;
			}
		}

		Splited Read(istream& in_stream) {
			return { PartionRead(Mode::MODIFY, in_stream),
				PartionRead(Mode::READ, in_stream) };
		}

		vector<string> Process(const Splited& requests) {
			vector<string> responses;
			BusManager manager;

			for (const auto& request_holder : requests.modify) {
				const auto& request = static_cast<const IModify&>(*request_holder);
				request.Process(manager);
			}

			for (const auto& request_holder : requests.read) {
				const auto& request = static_cast<const IRead<string>&>(*request_holder);
				responses.push_back(request.Process(manager));
			}

			return responses;
		}

		void Print(const vector<string>& responses, ostream& os_stream) {
			for (const auto& response : responses) {
				os_stream << response << '\n';
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

	RequestHolder IRequest::Create(Type type) {
		switch (type) {
		case Type::ADD_STOP:
			return make_unique<AddStop>();
		case Type::ADD_BUS:
			return make_unique<AddBus>();
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
	void AddStop::ParseFrom(string_view input) {
		stop_name = string(ReadToken(input, ": "));
		latitude = ConvertToDouble(ReadToken(input, ", "));
		longitude = ConvertToDouble(input);
	}

	void AddStop::Process(BusManager& manager) const {
		manager.AddStop(stop_name, { latitude, longitude });
	}

	// AddBus definition
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

	void AddBus::Process(BusManager& manager) const {
		manager.AddBus(bus_name, stops);
	}

	// BusInfo definition
	void BusInfo::ParseFrom(string_view input) {
		bus_name = string(input);
	}

	string BusInfo::Process(const BusManager& manager) const {
		return manager.BusInfo(bus_name);
	}
}
