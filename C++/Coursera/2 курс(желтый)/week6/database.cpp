#include "database.h"
#include <algorithm>

using namespace std;

void Database::Add(const Date& date, const std::string& event)  // O(log)
{// метод добавления события по дате
	auto p = m_data[date].insert(event);
	if (p.second)
	{ // если вставка произошла
		m_order[date].push_back(p.first); // вставляем итератор в словарь порядка
	}
}

ostream& Database::Print(ostream& os) const { // O(n^2)
	for (const auto& [date, events] : m_order)
	{ // итерируемся по датам
		for (const auto& event : events)
		{ // итерируемся по событиям даты
			os << date << ' ' << *event << endl;
		}
	}
	return os;
}

size_t Database::RemoveIf(const function<bool(const Date&, const string&)> f)
{
	size_t result = 0;

	for (auto it = m_order.begin(); it != m_order.end(); ) // pair - Date, vector<it>
	{
			auto events = it->second; // вектор итераторов
			auto last = remove_if(events.begin(), events.end(), [f, it](auto p) { return f(it->first, *p); }); // получаем конец диапазона с оставшимися событиями
			result += distance(last, events.end()); // количество удаляемых

			if (distance(last, events.end()) == events.size())
			{ // если удаляем все
				m_data.erase(it->first); // сначало очищаем дату в первом словаре
				it = m_order.erase(it); // потом во втором
			}
			else
			{
				for (auto it_v = last; it_v != events.end(); ++it)
				{ // разыменовываем итератор вектора для получения итератора множества
					m_data[it->first].erase(*it_v);
				}
				events.erase(last, events.end()); // O(n)
				++it;
			}
	}

	return result;
}

vector<pair<const Date&, const string&>> Database::FindIf(std::function<bool(const Date&, const string&)> f) const
{
	vector<pair<const Date&, const string&>> result;
	for (const auto& [date, events] : m_order)
	{ // итерируемся по датам
		for (const auto& event : events)
		{ // итерируемся по событиям даты
			if (f(date, *event)) // если данные удовлетворяют предикату, заносим ссылки в результат
			{
				result.push_back({ date, *event });
			}
		}
	}
	return result;
}

string Database::Last(const Date& date) const
{
	auto it = m_order.upper_bound(date);
	if (it == m_order.begin()) // если дат меньше нету
	{
		return "No entries";
	}
	it = prev(it); // ищем нужную дату
	return static_cast<string>(it->first) + ' ' + *it->second.back(); // выводим дату и последнее добавленное событие
}


string ParseEvent(istream& is)
{
	while (isspace(is.peek())) // игнорируем начальные пробелы
	{
		is.ignore(1);
	}
	string result;
	getline(is, result);
	if (result.empty())
	{
		throw invalid_argument("Parse empty event");
	}
	return result;
}
