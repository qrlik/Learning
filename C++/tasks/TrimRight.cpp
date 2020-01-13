#include <iostream>
using namespace std;

#include "test_runner.h"
#include "profile.h"

/*
Напишите функцию, которая обрезает пробелы в конце переданной ей строки.
Функция должна быть написана в расчёте на работу с очень длинными строками с очень большим количеством пробелов, оптимизирована по количеству обращений к памяти.
Сигнатура: void TrimRight( char *s );
*/

void TrimRight(char* s) {
	if (!s) {
		return;
	}

	size_t len = strlen(s);

	int mask = *reinterpret_cast<int*>(string(4, ' ').data());

	int* new_ptr = reinterpret_cast<int*>(s + len % 4);

	for (size_t i = len / 4; i > 0; --i, len -= 4) {
		if (new_ptr[i - 1] != mask) {
			break;
		}
	}

	for (; len > 0; --len) {
		if (s[len - 1] != ' ') {
			break;
		}
	}

	s[len] = '\0';
}

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

template<typename Func>
void TestTrimTemplate(Func f)
{
	string test = "a b c   ";
	string test2 = "                  a";
	string test3 = "a                  ";
	string test4 = "                  ";
	f(test.data());
	f(test2.data());
	f(test3.data());
	f(test4.data());

	string test_check = "a b c";
	string test2_check = "                  a";
	string test3_check = "a";
	string test4_check = "";
	ASSERT_EQUAL(test.data(), test_check);
	ASSERT_EQUAL(test2.data(), test2_check);
	ASSERT_EQUAL(test3.data(), test3_check);
	ASSERT_EQUAL(test4.data(), test4_check);
}

void TestTrimRight()
{
	TestTrimTemplate(TrimRight);
}

void TestTrimRightStrlen()
{
	TestTrimTemplate(TrimRightStrlen);
}

void TestTrimRightFor()
{
	TestTrimTemplate(TrimRightFor);
}

void TestTrimRightWhile()
{
	TestTrimTemplate(TrimRightWhile);
}

void TestAll()
{
	TestRunner tr;
	RUN_TEST(tr, TestTrimRight);
	RUN_TEST(tr, TestTrimRightStrlen);
	RUN_TEST(tr, TestTrimRightFor);
	RUN_TEST(tr, TestTrimRightWhile);
}

int main()
{
	TestAll();

	{
		LOG_DURATION("Pointer");
		string test(1'000'000'000, ' ');
		TrimRight(test.data());
		cout << "Pointer result is '" << test.data() << "'" << endl;
	}

	{
		LOG_DURATION("Strlen");
		string test(1'000'000'000, ' ');
		TrimRightStrlen(test.data());
		cout << "Strlen result is '" << test.data() << "'" << endl;
	}

	{
		LOG_DURATION("For");
		string test(1'000'000'000, ' ');
		TrimRightFor(test.data());
		cout << "For result is '" << test.data() << "'" << endl;
	}

	{
		LOG_DURATION("While");
		string test(1'000'000'000, ' ');
		TrimRightWhile(test.data());
		cout << "While result is '" << test.data() << "'" << endl;
	}
	return 0;
}
