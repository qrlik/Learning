#pragma once
#include <string>
#include <vector>
#include <iostream>

enum class QueryType { NewBus, BusesForStop, StopsForBus, AllBuses }; // тип команды

struct Query // структура для хранения ввода
{
	QueryType type; // тип команды
	std::string bus; // маршрут
	std::string stop; // остановка
	std::vector<std::string> stops; // список остановок
};

std::istream& operator>>(std::istream& is, Query& q); // порядок считывания данных из потока
