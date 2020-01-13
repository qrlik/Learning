#include <iostream>
using namespace std;

#include "test_runner.h"
#include "profile.h"

/*
Напишите функцию, которая обрезает пробелы в конце переданной ей строки.
Функция должна быть написана в расчёте на работу с очень длинными строками с очень большим количеством пробелов, оптимизирована по количеству обращений к памяти.
Сигнатура: void TrimRight( char *s );
*/

void TrimRightStrlen(char* s) {
	if (!s) {
		return;
	}

	size_t len = strlen(s);
	for(; len > 0; --len) {
		if (s[len - 1] != ' ') {
			break;
		}
	}
	s[len] = '\0';
}

void TrimRightFor(char* s) {
	if (!s) {
		return;
	}

	char* last_space = nullptr;
	for (; *s != '\0'; ++s)
	{
		if (*s == ' ') {
			if (!last_space) {
				last_space = s;
			}
		}
		else {
			last_space = nullptr;
		}
	}

	if (last_space) {
		*last_space = '\0';
	}
}

void TrimRightWhile(char* s) {
	if (!s) {
		return;
	}

	char* last_space = nullptr;
	char symbol = *s;

	while (symbol != '\0') {
		if (*s == ' ') {
			if (!last_space) {
				last_space = s;
			}
		}
		else if (symbol != ' ') {
			last_space = nullptr;
		}
		++s;
		symbol = *s;
	}

	if (last_space) {
		*last_space = '\0';
	}
}

void TestTrimRightStrlen()
{
	string test = "a b c   ";
	string test2 = "      ";
	TrimRightStrlen(test.data());
	TrimRightStrlen(test2.data());

	string test_check = "a b c";
	string test2_check = "";
	ASSERT_EQUAL(test.data(), test_check);
	ASSERT_EQUAL(test2.data(), test2_check);
}

void TestTrimRightFor()
{
	string test = "a b c   ";
	string test2 = "      ";
	TrimRightFor(test.data());
	TrimRightFor(test2.data());

	string test_check = "a b c";
	string test2_check = "";
	ASSERT_EQUAL(test.data(), test_check);
	ASSERT_EQUAL(test2.data(), test2_check);
}

void TestTrimRightWhile()
{
	string test = "a b c   ";
	string test2 = "      ";
	TrimRightWhile(test.data());
	TrimRightWhile(test2.data());

	string test_check = "a b c";
	string test2_check = "";
	ASSERT_EQUAL(test.data(), test_check);
	ASSERT_EQUAL(test2.data(), test2_check);
}

void TestAll()
{
	TestRunner tr;
	RUN_TEST(tr, TestTrimRightStrlen);
	RUN_TEST(tr, TestTrimRightFor);
	RUN_TEST(tr, TestTrimRightWhile);
}

int main()
{
	TestAll();

	{
		LOG_DURATION("Strlen");
		string test(10'000'000'000, ' ');
		TrimRightStrlen(test.data());
		cout << "Strlen result is '" << test.data() << "'" << endl;
	}

	{
		LOG_DURATION("For");
		string test(10'000'000'000, ' ');
		TrimRightFor(test.data());
		cout << "Pointer result is '" << test.data() << "'" << endl;
	}

	{
		LOG_DURATION("While");
		string test(10'000'000'000, ' ');
		TrimRightWhile(test.data());
		cout << "Pointer result is '" << test.data() << "'" << endl;
	}
	return 0;
}
