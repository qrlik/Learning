#include <ctime>

#include "date.h"
#include "parsers.h"

using namespace std;

Date Date::FromString(std::string_view str)
{
    const int year = ConvertToInt(ReadToken(str, "-"));
    const int month = ConvertToInt(ReadToken(str, "-"));
    ValidateBounds(month, 1, 12);
    const int day = ConvertToInt(str);
    ValidateBounds(day, 1, 31);
    return { year, month, day };
}

string Date::ToString() const
{
    return to_string(year_) + '-' + to_string(month_) + '-' + to_string(day_);
}

time_t Date::AsTimestamp() const
{
    std::tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = day_;
    t.tm_mon = month_ - 1;
    t.tm_year = year_ - 1900;
    t.tm_isdst = 0;
    return mktime(&t);
}

int64_t Date::operator-(const Date& other) const
{
    return (AsTimestamp() - other.AsTimestamp()) / SECONDS_IN_DAY;
}
