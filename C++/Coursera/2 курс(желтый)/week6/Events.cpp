#include "Events.h"

using namespace std;

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

void Events::Add(const string& event)
{
	auto p = m_events.insert(event);
	if (p.second)
	{ // если вставка произошла
		m_order.push_back(event); // добавляем в вектор
	}
}

const vector<string>& Events::getAll() const
{
	return m_order;
}
