#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <map>

struct BusesForStopResponse // обработка списка маршрутов
{
	std::stringstream ss;
	BusesForStopResponse();
	BusesForStopResponse(const std::vector<std::string>& buses);
};

std::ostream& operator<<(std::ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse
{
	std::stringstream ss;
	StopsForBusResponse();
	StopsForBusResponse(const std::string& bus, const std::vector<std::string>& stops, const std::map<std::string, std::vector<std::string>>& stops_to_buses);
};

std::ostream& operator<<(std::ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse
{
	std::stringstream ss;
	AllBusesResponse();
	AllBusesResponse(const std::map<std::string, std::vector<std::string>> buses_to_stops);
};

std::ostream& operator<<(std::ostream& os, const AllBusesResponse& r);
