#include "responses.h"

BusesForStopResponse::BusesForStopResponse()
{ // по умолчанию остановки нет
	ss << "No stop";
}

BusesForStopResponse::BusesForStopResponse(const std::vector<std::string>& buses)
{ // список маршрутов через данную остановку
	for (const auto& bus : buses)
	{
		ss << bus << " ";
	}
}

std::ostream& operator<<(std::ostream& os, const BusesForStopResponse& r)
{
	return os << r.ss.str();
}

StopsForBusResponse::StopsForBusResponse()
{ // по умолчанию маршрута нет
	ss << "No bus";
}

StopsForBusResponse::StopsForBusResponse(const std::string& bus, const std::vector<std::string>& stops, const std::map<std::string, std::vector<std::string>>& stops_to_buses)
{ // для маршрута список остановок и для каждой остановки список других маршрутов, проходящих через неё
	bool first = true;
	for (const std::string& stop : stops)
	{
		if (!first)
		{
			ss << std::endl;
		}
		first = false;
		ss << "Stop " << stop << ": ";
		if (stops_to_buses.at(stop).size() == 1)
		{
			ss << "no interchange";
		}
		else
		{
			for (const std::string& other_bus : stops_to_buses.at(stop))
			{
				if (bus != other_bus)
				{
					ss << other_bus << " ";
				}
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const StopsForBusResponse& r)
{
	return os << r.ss.str();
}

AllBusesResponse::AllBusesResponse()
{ // по умолчанию маршрутов нет
	ss << "No buses";
}

AllBusesResponse::AllBusesResponse(const std::map<std::string, std::vector<std::string>>& buses_to_stops)
{ // все маршруты и их список остановок
	bool first = true;
	for (const auto& bus_item : buses_to_stops)
	{
		if (!first)
		{
			ss << std::endl;
		}
		first = false;
		ss << "Bus " << bus_item.first << ": ";
		for (const std::string& stop : bus_item.second)
		{
			ss << stop << " ";
		}
	}
}

std::ostream& operator<<(std::ostream& os, const AllBusesResponse& r)
{
	return os << r.ss.str();
}
