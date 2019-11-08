#pragma once

#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <sstream>

std::string ParseEvent(std::istream& is); // считывание события

class Events
{ // класс хранищий события и их порядок добавления
	std::set<std::string> m_events;
	std::vector<std::string> m_order;
public:
	void Add(const std::string& event);
	const std::vector<std::string>& getAll() const; // получение событий по порядку добавления
	template <typename Func> size_t RemoveIf(Func Predicate)
	{ // удаление по предикату
		size_t result = 0;

		auto last = m_order.begin();
		for (auto it = m_order.begin(); it != m_order.end(); ++it) // итерируемся по вектору в порядке добавления
		{
			if (!Predicate(*it))
			{ // имитируем remove_if из алгоритмов, но с добавлением else
				*last++ = *it;
			}
			else
			{ // сразу удаляем из множества
				m_events.erase(*it);
			}
		}

		result += distance(last, m_order.end()); // количество удаляемых
		m_order.erase(last, m_order.end()); // O(n)

		return result;
	}
};
