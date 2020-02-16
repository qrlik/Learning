#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <string>
#include <string_view>
#include <optional>

#include "graph.h"

class BusManager
{
	struct Stop_info {
		std::set<std::string_view> buses; // проходящие автобусы
		std::unordered_map<std::string_view, size_t> distances; // дистации до остановок
		double latitude; // широта
		double longitude; // долгота
		double GeoLength(const Stop_info& other) const;
		std::optional<size_t> RoadLength(std::string_view other) const;
	};

	struct Road_info {
		std::vector<double> distances; /* массив суффиксов от 0 до road_length */
		double curvature; // кривизна (дорожное / географическое)
	};
	Road_info ComputeRoadInfo(const std::vector<std::string_view>& stops) const;

	struct Bus_info {
		Road_info road_info;
		std::vector<std::string_view> stops; // маршрут
		size_t unique_amount; // уникальные остановки
		bool is_roundtrip;
	};

	std::unordered_map<std::string, Stop_info> stop_data;
	std::unordered_map<std::string, Bus_info> bus_data;

public:
	void AddStop(std::string name, const std::unordered_map<std::string, size_t>& distances,
		double latitude, double longitude);
	void AddBus(std::string name, const std::vector<std::string>& stops, bool is_roundtrip);
	const Stop_info* StopInfo(const std::string& name) const;
	const Bus_info* BusInfo(const std::string& name) const;

	std::vector<std::string> getNumberedStops() const;
	std::unordered_set<std::string> getBusesSet() const;

	const auto& getStopsData() const {
		return stop_data;
	}
	const auto& getBusesData() const {
		return bus_data;
	}
};
