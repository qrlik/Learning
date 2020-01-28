#pragma once

#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <string_view>

class BusManager
{
	struct Stop_info {
		std::set<std::string_view> buses;
		double latitude; // широта
		double longitude; // долгота

		double Length(const Stop_info& other) const;
	};

	struct Bus_info {
		std::vector<std::string_view> stops;
		double length;
		size_t unique_stops;
	};
	double CalculateLength(const std::vector<std::string_view>& stops) const;

	std::unordered_map<std::string, Stop_info> stop_data;
	std::unordered_map<std::string, Bus_info> bus_data;
public:
	void AddStop(std::string name, Stop_info info);
	void AddBus(std::string name, const std::vector<std::string>& stops);
	std::string StopInfo(std::string name) const;
	std::string BusInfo(std::string name) const;
};
