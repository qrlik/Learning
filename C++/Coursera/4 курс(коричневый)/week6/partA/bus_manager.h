#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>

class BusManager
{
	struct StopInfo {
		double latitude; // широта
		double longitude; // долгота
		double Length(const StopInfo& other) const;
	};

	struct BusInfo {
		std::vector<std::string_view> stops;
		double length;
		size_t unique_stops;
	};
	double CalculateLength(const std::vector<std::string_view>& stops) const;

	std::unordered_map<std::string, StopInfo> stop_data;
	std::unordered_map<std::string, BusInfo> bus_data;
public:
	void AddStop(std::string name, StopInfo info);
	void AddBus(std::string name, const std::vector<std::string>& stops);
	std::string BusInfo(std::string name) const;
};
