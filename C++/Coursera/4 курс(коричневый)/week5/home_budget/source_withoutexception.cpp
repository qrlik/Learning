#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string_view>
#include <algorithm>
#include <ctime>

#include "test_runner.h"

using namespace std;

const static int SECONDS_IN_DAY = 60 * 60 * 24;
const static int DAYS_IN_99Y = 36526;  // дней от 2000 до 2099 гг

class Date
{
    int16_t year_;
    int8_t month_;
    int8_t day_;
public:
    Date(int y, int m, int d) : year_(y), month_(m), day_(d) {}

    time_t AsTimestamp() const
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
};

int ComputeDaysDiff(const Date& date_to, const Date& date_from) {
  const time_t timestamp_to = date_to.AsTimestamp();
  const time_t timestamp_from = date_from.AsTimestamp();
  return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

class Budget
{
    vector<double> data_;
    const static Date start;
    const static double tax;

    int getIndex(const Date& date) const
    { // date должна быть больше 2000-01-01 по условию задачи
        return ComputeDaysDiff(date,start);
    }
public:
    Budget() : data_(DAYS_IN_99Y) {}

    void Earn(const Date& from, const Date& to, int value)
    {
        int first = getIndex(from);
        int last = getIndex(to);
        double day_income = static_cast<double>(value) / (last - first + 1); // диапазон [from, to]

        for (; first <= last; ++first)
        {
            data_[first] += day_income;
        }
    }

    void PayTax(const Date& from, const Date& to)
    {
        for (int i = getIndex(from); i <= getIndex(to); ++i)
        {
            data_[i] *= tax;
        }
    }

    double ComputeIncome(const Date& from, const Date& to) const
    {
        double result = 0;

        for (int i = getIndex(from); i <= getIndex(to); ++i)
        {
            result += data_[i];
        }

        return result;
    }
};

const Date Budget::start = { 2000, 1, 1 };
const double Budget::tax = 0.87;

Date ParseDate(string_view str)
{ // Все даты вводятся в формате YYYY-MM-DD. Даты корректны
    int16_t year = stoi(string(str.substr(0, 4)));
    str.remove_prefix(5);
    int8_t month = stoi(string(str.substr(0, 2)));
    str.remove_prefix(3);
    int8_t day = stoi(string(str.substr(0, 2)));;
    return Date{ year, month, day };
}

vector<string_view> SplitIntoWords(string_view str)
{
    vector<string_view> result;

    while (true) {
        size_t not_space = str.find_first_not_of(' '); // лишние пробелы перед словом
        str.remove_prefix(not_space);

        size_t space = str.find(' ');
        result.push_back(str.substr(0, space));
        if (space == str.npos)
        {
            break;
        }
        else
        {
            str.remove_prefix(space + 1);
        }
    }

    return result;
}

struct Request
{
    string command;
    Date from;
    Date to;
    int value = -1; //положительные целые числа, не превышающие 10^6
};

Request ProcessRequest(string_view request)
{
    vector<string_view> words = SplitIntoWords(request);
    if (words.size() >= 3 && words.size() < 5)
    {
        Request result{ string(words[0]), ParseDate(words[1]), ParseDate(words[2]) };
        if (words.size() == 4)
        {
            result.value = stoi(string(words[3]));
        }
        return result;
    }
    else
    {
        throw invalid_argument("Wrong size of request in ProcessRequest()");
    }
}

void PrintRequest(Request request, Budget& budget, ostream& os = cout)
{
    if (request.command == "ComputeIncome")
    {
        os << budget.ComputeIncome(request.from, request.to) << '\n';
    }
    else if (request.command == "Earn")
    {
        budget.Earn(request.from, request.to, request.value);
    }
    else if (request.command == "PayTax")
    {
        budget.PayTax(request.from, request.to);
    }
    else
    {
        throw invalid_argument("Wrong request command in PrintRequest()");
    }
}

vector<string> ParseRequests(istream& is = cin)
{
    int count;
    is >> count;
    is.ignore(1);

    vector<string> requests;
    string request;

    for (int i = 0; i < count && getline(is, request); ++i)
    {
        if (!request.empty())
        {
            requests.push_back(move(request));
        }
    }

    return requests;
}

void TaskTest()
{
    istringstream input("8\n"
        "Earn 2000-01-02 2000-01-06 20\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "PayTax 2000-01-02 2000-01-03\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "Earn 2000-01-03 2000-01-03 10\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "PayTax 2000-01-03 2000-01-03\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
    );

    vector<string> requests = ParseRequests(input);
    ostringstream output;
    Budget b;
    for (const string& request : requests)
    {
        PrintRequest(ProcessRequest(request), b, output);
    }
    ASSERT_EQUAL(output.str(), "20\n"
        "18.96\n"
        "28.96\n"
        "27.2076\n");
}

void TestAll()
{
    TestRunner tr;
    RUN_TEST(tr, TaskTest);
}

int main() {
    TestAll();

    vector<string> requests = ParseRequests();
    Budget home_budget;

    try
    {
        for (const string& request : requests)
        {
            cout.precision(25);
            PrintRequest(ProcessRequest(request), home_budget);
        }
    }
    catch (const invalid_argument & ex)
    {
        cerr << ex.what() << endl;
    }
    catch (...)
    {
        cerr << "Unexpect exception" << endl;
    }

    return 0;
}
