#include "request.h"
#include "test_runner.h"

using namespace std;

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

    const auto requests = ReadRequests(input);
    const auto responses = ProcessRequests(requests);
    ostringstream output;
    PrintResponses(responses, output);
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

    const auto requests = ReadRequests(input);
    const auto responses = ProcessRequests(requests);
    ostringstream output;
    PrintResponses(responses, output);
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

    try
    {
        cout.precision(25);
        const auto requests = ReadRequests();
        const auto responses = ProcessRequests(requests);
        PrintResponses(responses);
    }
    catch (const exception& ex)
    {
        cerr << ex.what() << endl;
    }
    catch (...)
    {
        cerr << "Unexpect exception" << endl;
    }

    return 0;
}
