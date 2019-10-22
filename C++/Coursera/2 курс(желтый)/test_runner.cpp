#include "test_runner.h"

void Assert(bool b, const std::string& hint) // проверка на логическое соответствие
{
	AssertEqual(b, true, hint);
}

template <class TestFunc> void TestRunner::RunTest(TestFunc func, const std::string& test_name) // шаблонный запуск тестирующей функции
{
	try
	{
		func();
		std::cerr << test_name << " OK" << std::endl;
	}
	catch (std::exception & e)
	{
		++fail_count;
		std::cerr << test_name << " fail: " << e.what() << std::endl;
	}
	catch (...)
	{
		++fail_count;
		std::cerr << "Unknown exception caught" << std::endl;
	}
}

TestRunner::~TestRunner() // проверка на верность тестов
{
	if (fail_count > 0)
	{
		std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
		exit(1);
	}
}
