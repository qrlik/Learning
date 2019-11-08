#include "date.h"
#include <tuple>

using namespace std;

Date::Date(int year, int month, int day) : m_year(year), m_month(month), m_day(day) {}

bool Date::operator<(const Date& rhs) const
{
	return tie(m_year, m_month, m_day) < tie(rhs.m_year, rhs.m_month, rhs.m_day);
}

bool Date::operator<=(const Date& rhs) const
{
	return tie(m_year, m_month, m_day) <= tie(rhs.m_year, rhs.m_month, rhs.m_day);
}

bool Date::operator>(const Date& rhs) const
{
	return tie(m_year, m_month, m_day) > tie(rhs.m_year, rhs.m_month, rhs.m_day);
}

bool Date::operator>=(const Date& rhs) const
{
	return tie(m_year, m_month, m_day) >= tie(rhs.m_year, rhs.m_month, rhs.m_day);
}

bool Date::operator==(const Date& rhs) const
{
	return tie(m_year, m_month, m_day) == tie(rhs.m_year, rhs.m_month, rhs.m_day);
}

bool Date::operator!=(const Date& rhs) const
{
	return tie(m_year, m_month, m_day) != tie(rhs.m_year, rhs.m_month, rhs.m_day);
}

ostream& operator<<(ostream& os, const Date& date)
{ // Вам не понадобится выводить дату с отрицательным значением года
	os << setfill('0') << setw(4) << date.GetYear() << '-'
		<< setw(2) << date.GetMonth() << '-'
		<< setw(2) << date.GetDay();
	return os;
}

Date ParseDate(istream& is)
{
	int y, m, d;
	char c1, c2;
	is >> y >> c1 >> m >> c2 >> d; // считываем данные
	if (is && c1 == '-' && c2 == '-') // если считалось корректорно
	{// 1. была ли запись во все переменные 2. далее ничего не было 3. верные разделители
		if (m < 1 || m > 12)
		{ // проверяем месяц
			throw invalid_argument("Month value is invalid: " + to_string(m));
		}
		else if (d < 1 || d > 31)
		{ // проверяем дату
			throw invalid_argument("Day value is invalid: " + to_string(d));
		} // задаем переменные
		return Date(y, m, d);
	}
	else
	{
		throw invalid_argument("Wrong date format");
	}
}
