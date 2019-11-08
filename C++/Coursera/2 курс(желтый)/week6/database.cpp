#include "database.h"
#include <algorithm>

using namespace std;

void Database::Add(const Date& date, const string& event)  // O(log)
{// метод добавления события по дате
	m_data[date].Add(event);
}

ostream& Database::Print(ostream& os) const { // O(n^2)
	for (const auto& [date, events] : m_data)
	{ // итерируемся по датам
		for (const auto& event : events.getAll())
		{ // итерируемся по событиям даты
			os << date << ' ' << event << endl;
		}
	}
	return os;
}

string Database::Last(const Date& date) const
{
	auto it = m_data.upper_bound(date);
	if (it == m_data.begin()) // если дат меньше нету
	{
		return "No entries";
	}
	it = prev(it); // ищем нужную дату
	ostringstream os;
	os << it->first;
	return os.str() + ' ' + it->second.getAll().back(); // выводим дату и последнее добавленное событие
}
