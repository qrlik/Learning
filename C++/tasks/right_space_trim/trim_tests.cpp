#include <iostream>
#include <string_view>
#include <algorithm>
#include <optional>
using namespace std;

#include "test_runner.h"
#include "profile.h"

/*
Напишите функцию, которая обрезает пробелы в конце переданной ей строки.
Функция должна быть написана в расчёте на работу с очень длинными строками с очень большим количеством пробелов, оптимизирована по количеству обращений к памяти.
Сигнатура: void TrimRight( char *s );
*/

template<typename Func>
void TestTrimTemplate(Func f)
{
	string test = "a b c   ";
	string test2 = "                  a";
	string test3 = "a                  ";
	string test4 = "                  ";
	string test5 = "a";
	string test6 = "";
	string test7 = "a b c ";
	string test8 = "ab ";
	string test9 = " ab";

	f(test.data());
	f(test2.data());
	f(test3.data());
	f(test4.data());
	f(test5.data());
	f(test6.data());
	f(test7.data());
	f(test8.data());
	f(test9.data());

	string test_check = "a b c";
	string test2_check = "                  a";
	string test3_check = "a";
	string test4_check = "";
	string test5_check = "a";
	string test6_check = "";
	string test7_check = "a b c";
	string test8_check = "ab";
	string test9_check = " ab";

	ASSERT_EQUAL(test.data(), test_check);
	ASSERT_EQUAL(test2.data(), test2_check);
	ASSERT_EQUAL(test3.data(), test3_check);
	ASSERT_EQUAL(test4.data(), test4_check);
	ASSERT_EQUAL(test5.data(), test5_check);
	ASSERT_EQUAL(test6.data(), test6_check);
	ASSERT_EQUAL(test7.data(), test7_check);
	ASSERT_EQUAL(test8.data(), test8_check);
	ASSERT_EQUAL(test9.data(), test9_check);
}

void TestTrimRight()
{
	TestTrimTemplate(TrimRight);
}

void TestAll()
{
	TestRunner tr;
	RUN_TEST(tr, TestTrimRight);
}

int main()
{
	TestAll();

	{
		LOG_DURATION("Main");
		string test(1'000'000'000, ' ');
		TrimRight(test.data());
		cout << "Main result is '" << test.data() << "'" << endl;
	}
	return 0;
}
