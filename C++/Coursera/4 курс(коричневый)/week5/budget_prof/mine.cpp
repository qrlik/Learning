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
    Date() = default;
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

    int64_t operator-(const Date& other) const
    {
        return (AsTimestamp() - other.AsTimestamp()) / SECONDS_IN_DAY;
    }
};

class Budget
{
    struct DayBudget
    {
        double income;
        double spending;
        double ComputeIncome() const
        {
            return income - spending;
        }
    };
    vector<DayBudget> data_;
    const static Date start;

    int getIndex(const Date& date) const
    { // date должна быть больше 2000-01-01 по условию задачи
        int64_t result = date - start;
        if (result < 0)
        {
            throw invalid_argument("Wrong date in getIndex() -> date < 2000-01-01");
        }
        else if (result >= static_cast<int64_t>(data_.size()))
        {
            throw invalid_argument("Wrong date in getIndex() -> date > 2100-01-01");
        }
        return static_cast<int>(result);
    }
public:
    Budget() : data_(DAYS_IN_99Y) {}

    void Earn(const Date& from, const Date& to, int value)
    {
        int first = getIndex(from);
        const int last = getIndex(to); 
        double day_income = static_cast<double>(value) / (last - first + 1); // диапазон [from, to]

        for (; first <= last; ++first)
        {
            data_[first].income += day_income;
        }
    }

    void PayTax(const Date& from, const Date& to, int percentage)
    {
        const double tax_percent = (100.0 - percentage) / 100; // Гарантируется, что percentage — целое число от 0 до 100.
        const int last = getIndex(to);
        for (int first = getIndex(from); first <= last; ++first)
        {
            data_[first].income *= tax_percent;
        }
    }

    void Spend(const Date& from, const Date& to, int value)
    {
        int first = getIndex(from);
        const int last = getIndex(to);
        double day_spend = static_cast<double>(value) / (last - first + 1); // диапазон [from, to]

        for (; first <= last; ++first)
        {
            data_[first].spending += day_spend;
        }
    }

    double ComputeIncome(const Date& from, const Date& to) const
    {
        double result = 0;
        const int last = getIndex(to);

        for (int first = getIndex(from); first <= last; ++first)
        {
            result += data_[first].ComputeIncome();
        }

        return result;
    }
};

const Date Budget::start = { 2000, 1, 1 };

istream& operator>>(istream& is, Date& date)
{ // Все даты вводятся в формате YYYY-MM-DD. Даты корректны
    int year;
    int month, day;
    is >> year;
    is.ignore(1);
    is >> month;
    is.ignore(1);
    is >> day;
    date = Date(year, month, day);
    return is;
}

struct Request
{
    string command;
    Date from;
    Date to;
    int value;
};

Request ProcessRequest(const string& request)
{
    istringstream is(request);
    string command;
    Date from, to;
    int value = -1;
    is >> command >> from >> to >> value;
    return Request{move(command), from, to, value};
}

void PrintRequest(Request request, Budget& budget, ostream& os = cout)
{
    if (request.command == "Earn")
    {
    budget.Earn(request.from, request.to, request.value);
    }
    else if (request.command == "PayTax")
    {
        budget.PayTax(request.from, request.to, request.value);
    }
    else if (request.command == "Spend")
    {
        budget.Spend(request.from, request.to, request.value);
    }
    else if (request.command == "ComputeIncome")
    {
        os << budget.ComputeIncome(request.from, request.to) << '\n';
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

void TaskTest1()
{
    istringstream input("8\n"
        "Earn 2000-01-02 2000-01-06 20\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "PayTax 2000-01-02 2000-01-03 13\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "Earn 2000-01-03 2000-01-03 10\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "PayTax 2000-01-03 2000-01-03 13\n"
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

void TaskTest2()
{
    istringstream input("8\n"
        "Earn 2000-01-02 2000-01-06 20\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "PayTax 2000-01-02 2000-01-03 13\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "Spend 2000-12-30 2001-01-02 14\n"
        "ComputeIncome 2000-01-01 2001-01-01\n"
        "PayTax 2000-12-30 2000-12-30 13\n"
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
        "8.46\n"
        "8.46\n");
}

void TestAll()
{
    TestRunner tr;
    RUN_TEST(tr, TaskTest1);
    RUN_TEST(tr, TaskTest2);
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
