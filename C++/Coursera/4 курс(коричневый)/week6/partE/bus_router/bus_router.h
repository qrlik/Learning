#pragma once

#include <unordered_set>

#include "bus_manager.h"
#include "router.h"

class BusRouter {
	struct GraphData {
		using Weight = double; // wait + road_time
		Weight CalculateWeight(double length) const;

		Graph::DirectedWeightedGraph<Weight> graph;
		std::unordered_set<std::string> buses;
		std::vector<std::string> stops;
		std::unordered_map<std::string_view, size_t> stops_to_id;

		struct Edge_info {
			std::string_view bus_name;
			size_t span_count;
		};
		std::vector<Edge_info> edges;

		const double bus_velocity;
		const uint16_t bus_wait_time;
		GraphData(const BusManager& manager, double bus_velocity, uint16_t bus_wait_time);
		void AddRoundtripBus(std::string_view bus, const std::vector<size_t>& stops_id, const std::vector<double>& distances);
		void AddNotRoundtripBus(std::string_view bus, const std::vector<size_t>& stops_id, const std::vector<double>& distances);
	};

	const GraphData data;
	const Graph::Router<GraphData::Weight> router;
public:
	BusRouter(const BusManager& manager, double bus_velocity, uint16_t bus_wait_time);

	struct Route_info {
		struct Trip_info {
			std::string bus_name;
			std::string from_stop_name;
			double bus_time;
			size_t span_count;
			uint16_t wait_time;
		};
		std::vector<Trip_info> route;
		GraphData::Weight total_time;
	};
	std::optional<Route_info> RouteInfo(const std::string& from, const std::string& to) const;
};
