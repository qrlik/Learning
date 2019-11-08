#pragma once

#include "date.h"
#include "Events.h"
#include <map>
#include <set>
#include <vector>
#include <functional>

class Database
{
private:
	std::map<Date, Events> m_data; // словарь событий
public:
	void Add(const Date& date, const std::string& event); // метод добавления события по дате
	template <typename Func> size_t RemoveIf(Func Predicate) // удаление по предикату
	{
		size_t result = 0;

		for (auto it = m_data.begin(); it != m_data.end(); ) // pair - Date, Events
		{
			result += it->second.RemoveIf([=](const std::string& event) { return Predicate(it->first, event); });

			it = (it->second.getAll().empty()) ? m_data.erase(it) : next(it); // если удалились все события удаляем дату
		}

		return result;
	}
	template <typename Func> std::vector<std::pair<const Date&, const std::string&>> FindIf(Func Predicate) const // поиск по предикату
	{
		std::vector<std::pair<const Date&, const std::string&>> result;
		for (const auto& [date, events] : m_data)
		{ // итерируемся по датам
			for (const auto& event : events.getAll())
			{ // итерируемся по событиям даты
				if (Predicate(date, event)) // если данные удовлетворяют предикату, заносим ссылки в результат
				{
					result.push_back({ date, event });
				}
			}
		}
		return result;
	}
	std::ostream& Print(std::ostream& os) const; // печать всей базы
	std::string Last(const Date& date) const; // последнее событие к дате
};
