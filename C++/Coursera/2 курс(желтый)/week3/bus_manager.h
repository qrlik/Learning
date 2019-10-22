#pragma once
#include <string>
#include <vector>
#include <map>
#include "responses.h"

class BusManager // хранилище данных и работа с ними
{
public:
	void AddBus(const std::string& bus, const std::vector<std::string>& stops); // добавление маршрута + списка остановок
	BusesForStopResponse GetBusesForStop(const std::string& stop) const;
	StopsForBusResponse GetStopsForBus(const std::string& bus) const;
	AllBusesResponse GetAllBuses() const;
private:
	std::map<std::string, std::vector<std::string>> buses_to_stops;
	std::map<std::string, std::vector<std::string>> stops_to_buses;
};

