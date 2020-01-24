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

int ComputeDaysDiff(const Date& date_to, const Date& date_from) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();
    return static_cast<int>(static_cast<int64_t>(timestamp_to - timestamp_from) / SECONDS_IN_DAY);
}
