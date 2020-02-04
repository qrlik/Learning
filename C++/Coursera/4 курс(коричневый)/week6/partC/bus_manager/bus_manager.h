#pragma once

#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <string_view>
#include <optional>

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

	struct Lenght_info {
		double road_length; // географическая дистанция
		double curvature; // кривизна (дорожное / географическое)
	};
	Lenght_info CalculateLength(const std::vector<std::string_view>& stops) const;

	struct Bus_info {
		std::vector<std::string_view> stops; // маршрут
		Lenght_info length;
		size_t unique_stops;
	};

	std::unordered_map<std::string, Stop_info> stop_data;
	std::unordered_map<std::string, Bus_info> bus_data;
public:
	void AddStop(std::string name, const std::unordered_map<std::string, size_t>& distances,
		double latitude, double longitude);
	void AddBus(std::string name, const std::vector<std::string>& stops);
	std::string StopInfo(std::string name) const;
	std::string BusInfo(const std::string& name) const;
};
