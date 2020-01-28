
#include "test_runner.h"
#include "request.h"

using namespace std;

void TestPartA() {
    istringstream input("10\n"
        "Stop Tolstopaltsevo: 55.611087, 37.20829\n"
        "Stop Marushkino: 55.595884, 37.209755\n"
        "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
        "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
        "Stop Rasskazovka: 55.632761, 37.333324\n"
        "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n"
        "Stop Biryusinka: 55.581065, 37.64839\n"
        "Stop Universam: 55.587655, 37.645687\n"
        "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
        "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
        "3\n"
        "Bus 256\n"
        "Bus 750\n"
        "Bus 751\n"
    );

    using namespace Request::Handler;
    const auto requests = Read(input);
    const auto responses = Process(requests);
    ostringstream output;
    Print(responses, output);
    ASSERT_EQUAL(output.str(),
        "Bus 256: 6 stops on route, 5 unique stops, 4371.017251 route length\n"
        "Bus 750: 5 stops on route, 3 unique stops, 20939.483047 route length\n"
        "Bus 751: not found\n"
    );
}

void TestPartB() {
    istringstream input("13\n"
        "Stop Tolstopaltsevo: 55.611087, 37.20829\n"
        "Stop Marushkino: 55.595884, 37.209755\n"
        "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
        "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
        "Stop Rasskazovka: 55.632761, 37.333324\n"
        "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n"
        "Stop Biryusinka: 55.581065, 37.64839\n"
        "Stop Universam: 55.587655, 37.645687\n"
        "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
        "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
        "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
        "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
        "Stop Prazhskaya: 55.611678, 37.603831\n"
        "6\n"
        "Bus 256\n"
        "Bus 750\n"
        "Bus 751\n"
        "Stop Samara\n"
        "Stop Prazhskaya\n"
        "Stop Biryulyovo Zapadnoye\n"
    );

    using namespace Request::Handler;
    const auto requests = Read(input);
    const auto responses = Process(requests);
    ostringstream output;
    Print(responses, output);
    ASSERT_EQUAL(output.str(),
        "Bus 256: 6 stops on route, 5 unique stops, 4371.017251 route length\n"
        "Bus 750: 5 stops on route, 3 unique stops, 20939.483047 route length\n"
        "Bus 751: not found\n"
        "Stop Samara: not found\n"
        "Stop Prazhskaya: no buses\n"
        "Stop Biryulyovo Zapadnoye: buses 256 828\n"
    );
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestPartA);
    RUN_TEST(tr, TestPartB);
}

int main() {
    TestAll();

    using namespace Request::Handler;
    const auto requests = Read();
    const auto responses = Process(requests);
    Print(responses);
    return 0;
}
