#include "test_runner.h"

void Assert(bool b, const std::string& hint) // проверка на логическое соответствие
{
	AssertEqual(b, true, hint);
}

TestRunner::~TestRunner() // проверка на верность тестов
{
	if (fail_count > 0)
	{
		std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
		exit(1);
	}
}
