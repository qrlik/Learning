#include "bus_manager.h"

void BusManager::AddBus(const std::string& bus, const std::vector<std::string>& stops) // добавление маршрута + списка остановок
{
	buses_to_stops[bus] = stops;
	for (const std::string& stop : stops)
	{
		stops_to_buses[stop].push_back(bus);
	}
}

BusesForStopResponse BusManager::GetBusesForStop(const std::string& stop) const
{
	if (stops_to_buses.count(stop) == 0)
	{ // если остановки нет вызываем BusesForStopResponse()
		return {};
	}
	return { stops_to_buses.at(stop) }; // иначе возвращаем обработанный список
}

StopsForBusResponse BusManager::GetStopsForBus(const std::string& bus) const
{
	if (buses_to_stops.count(bus) == 0)
	{ // если маршрута нет вызываем StopsForBusResponse()
		return {};
	}
	return { bus, buses_to_stops.at(bus), stops_to_buses }; // иначе возвращаем обработанный список
}

AllBusesResponse BusManager::GetAllBuses() const
{
	if (buses_to_stops.empty())
	{ // если маршрутов нет вызываем AllBusesResponse()
		return {};
	}
	return { buses_to_stops }; // иначе возвращаем обработанный список
}
