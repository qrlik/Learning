#include <cassert>

#include "test_runner.h"
#include "mentor_algorithm.h"

using namespace std;

void PrintResults(istream& is = cin, ostream& os = cout)
{
    Init();

    int q;
    is >> q;

    for (int i = 0; i < q; ++i) {
        string query_type;
        is >> query_type;

        string date_from_str, date_to_str;
        is >> date_from_str >> date_to_str;

        auto idx_from = ComputeDayIndex(Date::FromString(date_from_str));
        auto idx_to = ComputeDayIndex(Date::FromString(date_to_str)) + 1;

        if (query_type == "ComputeIncome") {
            os << ComputeSum(1, 0, DAY_COUNT_P2, idx_from, idx_to) << endl;
        }
        else if (query_type == "PayTax") {
            int percentage;
            is >> percentage;
            const double factor = 1.0 - percentage / 100.0;
            Multiply(1, 0, DAY_COUNT_P2, idx_from, idx_to, factor);
        }
        else if (query_type == "Earn") {
            double value;
            is >> value;
            const double income = value / (idx_to - idx_from);
            Add(1, 0, DAY_COUNT_P2, idx_from, idx_to, MoneyState{ income, 0.0 });
        }
        else if (query_type == "Spend") {
            double value;
            is >> value;
            const double spend = value / (idx_to - idx_from);
            Add(1, 0, DAY_COUNT_P2, idx_from, idx_to, MoneyState{ 0.0, spend });
        }
    }
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
    ostringstream output;
    PrintResults(input, output);
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

    ostringstream output;
    PrintResults(input, output);
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

    cout.precision(25);
    assert(DAY_COUNT <= DAY_COUNT_P2 && DAY_COUNT_P2 < DAY_COUNT * 2);

    PrintResults();
    return 0;
}
