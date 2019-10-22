#include "query.h"

std::istream& operator >> (std::istream& is, Query& q) // порядок считывания данных из потока
{
	std::string operation_code;
	is >> operation_code;
	if (operation_code == "NEW_BUS")
	{
		q.type = QueryType::NewBus;
		int stop_count = 0;
		std::cin >> q.bus >> stop_count;
		q.stops.resize(stop_count);
		for (std::string& stop : q.stops)
		{
			is >> stop;
		}
	}
	else if (operation_code == "BUSES_FOR_STOP")
	{
		q.type = QueryType::BusesForStop;
		is >> q.stop;
	}
	else if (operation_code == "STOPS_FOR_BUS")
	{
		q.type = QueryType::StopsForBus;
		is >> q.bus;
	}
	else if (operation_code == "ALL_BUSES")
	{
		q.type = QueryType::AllBuses;
	}
	return is;
} // считываем данные команды
