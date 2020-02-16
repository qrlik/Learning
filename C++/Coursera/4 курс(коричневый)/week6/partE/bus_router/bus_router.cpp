#include "bus_router.h"

#include <algorithm>
#include <iterator>

using namespace std;
using namespace Graph;

namespace {
	const double KM_H_TO_M_MIN = 1000.0 / 60;

	unordered_map<string_view, size_t> BuildStopsToId(const vector<string>& stops) {
		unordered_map<string_view, size_t> stops_to_id;
		for (size_t i = 0; i < stops.size(); ++i) {
			stops_to_id[stops[i]] = i;
		}
		return stops_to_id;
	}

	vector<size_t> ConvertStopsToId(const vector<string_view>& stops, const unordered_map <string_view, size_t>& stop_to_id) {
		vector<size_t> stops_id;
		stops_id.reserve(stops.size());

		for (string_view stop : stops) {
			stops_id.emplace_back(stop_to_id.at(stop));
		}

		return stops_id;
	}
}

/* GraphData realization */
double BusRouter::GraphData::CalculateWeight(double length) const {
	return length / bus_velocity + bus_wait_time;
}

BusRouter::GraphData::GraphData(const BusManager& manager, double bus_velocity, uint16_t bus_wait_time):
	graph(manager.getStopsData().size()), buses(manager.getBusesSet()), stops(manager.getNumberedStops()),
	stops_to_id(BuildStopsToId(stops)), bus_velocity(bus_velocity * KM_H_TO_M_MIN), bus_wait_time(bus_wait_time) {

	const auto& bus_data = manager.getBusesData();
	for (const auto& bus : buses) {
		const auto& bus_info = bus_data.at(bus);
		vector<size_t> stops_id = ConvertStopsToId(bus_info.stops, stops_to_id);

		if (bus_info.is_roundtrip) {
			AddRoundtripBus(bus, stops_id, bus_info.road_info.distances);
		}
		else {
			AddNotRoundtripBus(bus, stops_id, bus_info.road_info.distances);
		}
	}
}

void BusRouter::GraphData::AddRoundtripBus(string_view bus, const std::vector<size_t>& stops_id, const std::vector<double>& distances) {
	for (size_t to = 1; to + 1 < stops_id.size(); ++to) { /* from = 0 */
		Weight weight = CalculateWeight(distances[to]); /* wait -> from -> to */
		graph.AddEdge({ .from = stops_id[0], .to = stops_id[to], .weight = weight });
		edges.emplace_back(Edge_info{ bus, to });
	}

	for (size_t from = 1; from + 1 < stops_id.size(); ++from) { /* добавляем все возможные ребра от остановки */
		for (size_t to = from + 1; to < stops_id.size(); ++to) {
			double weight = CalculateWeight(distances[to] - distances[from]); /* wait -> from -> to */
			graph.AddEdge({ .from = stops_id[from], .to = stops_id[to], .weight = weight });
			edges.emplace_back(Edge_info{ bus, to - from });
		}
	}
}

void BusRouter::GraphData::AddNotRoundtripBus(string_view bus, const std::vector<size_t>& stops_id, const std::vector<double>& distances) {
	for (size_t from = 0; from < stops_id.size() / 2; ++from) {
		for (size_t to = from + 1; to <= stops_id.size() / 2; ++to) {
			double weight = CalculateWeight(distances[to] - distances[from]);
			graph.AddEdge({ .from = stops_id[from], .to = stops_id[to], .weight = weight });
			edges.emplace_back(Edge_info{ bus, to - from });
		}
	}

	for (size_t from = stops_id.size() / 2; from + 1 < stops_id.size(); ++from) {
		for (size_t to = from + 1; to < stops_id.size(); ++to) {
			double weight = CalculateWeight(distances[to] - distances[from]);
			graph.AddEdge({ .from = stops_id[from], .to = stops_id[to], .weight = weight });
			edges.emplace_back(Edge_info{ bus, to - from });
		}
	}
}

/* BusRouter realization */
BusRouter::BusRouter(const BusManager& manager, double bus_velocity, uint16_t bus_wait_time) :
	data(manager, bus_velocity, bus_wait_time), router(data.graph) {}

optional<BusRouter::Route_info> BusRouter::RouteInfo(const string& from, const string& to) const {
	auto get_route = router.BuildRoute(data.stops_to_id.at(from), data.stops_to_id.at(to));
	if (!get_route) {
		return nullopt;
	}
	const auto& route_info = get_route.value();

	Route_info result;
	result.total_time = route_info.weight;
	result.route.reserve(route_info.edge_count);

	for (size_t edge = 0; edge < route_info.edge_count; ++edge) {
		EdgeId edge_id = router.GetRouteEdge(route_info.id, edge);
		const auto& edge_data = data.graph.GetEdge(edge_id);

		result.route.emplace_back(Route_info::Trip_info{ .bus_name = string(data.edges[edge_id].bus_name),
			.from_stop_name = data.stops[edge_data.from], .bus_time = edge_data.weight - data.bus_wait_time,
			.span_count = data.edges[edge_id].span_count, .wait_time = data.bus_wait_time });
	}

	return result;
}
