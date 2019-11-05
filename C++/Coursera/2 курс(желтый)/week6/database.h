#pragma once

#include "date.h"
#include <map>
#include <set>
#include <vector>
#include <functional>

class Database
{
private:
	std::map<Date, std::set<std::string>> m_data; // храним уникальные значение
	std::map<Date, std::vector<std::set<std::string>::const_iterator>> m_order; // храним порядок
public:
	void Add(const Date& date, const std::string& event); // метод добавления события по дате
	std::ostream& Print(std::ostream& os) const; // печать всей базы
	size_t RemoveIf(const std::function<bool(const Date&, const std::string&)> f); // удаление по предикату
	std::vector<std::pair<const Date&, const std::string&>> FindIf(std::function<bool(const Date&, const std::string&)> f) const; // поиск по предикату
	std::string Last(const Date& date) const; // последнее событие к дате
};

std::string ParseEvent(std::istream& is);
