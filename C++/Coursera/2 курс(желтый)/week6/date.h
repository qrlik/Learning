#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

class Date
{
private:
	int m_year;
	int m_month;
	int m_day;
public:
	Date(int year, int month, int day);
	int GetYear() const { return m_year; }
	int GetMonth() const { return m_month; }
	int GetDay() const { return m_day; }
	bool operator<(const Date& rhs) const;
	bool operator<=(const Date& rhs) const;
	bool operator>(const Date& rhs) const;
	bool operator>=(const Date& rhs) const;
	bool operator==(const Date& rhs) const;
	bool operator!=(const Date& rhs) const;
};

std::ostream& operator<<(std::ostream& os, const Date& date);
Date ParseDate(std::istream& str);
