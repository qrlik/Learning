#pragma once
#include <string_view>

static constexpr int SECONDS_IN_DAY = 60 * 60 * 24;

class Date
{
	int year_;
	int month_;
	int day_;
	Date(int year, int month, int day) :
		year_(year), month_(month), day_(day) {}
public:
	static Date FromString(std::string_view str);
	std::string ToString() const;
	time_t AsTimestamp() const;
};

int ComputeDaysDiff(const Date& date_to, const Date& date_from);
