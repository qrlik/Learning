#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>

template <class T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& s) // шаблонный вывод вектора
{
	os << "[";
	bool first = true;
	for (const auto& x : s)
	{
		if (!first)
		{
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "]";
}

template <class T> std::ostream& operator<<(std::ostream& os, const std::set<T>& s) // шаблонный вывод множества
{
	os << "{";
	bool first = true;
	for (const auto& x : s)
	{
		if (!first)
		{
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class K, class V> std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) // шаблонный вывод карты
{
	os << "{";
	bool first = true;
	for (const auto& kv : m)
	{
		if (!first)
		{
			os << ", ";
		}
		first = false;
		os << kv.first << ": " << kv.second;
	}
	return os << "}";
}

template<class T, class U> void AssertEqual(const T& t, const U& u, const std::string& hint = {}) // проверка на равенство объектов
{
	if (t != u)
	{
		std::ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty()) {
			os << " hint: " << hint;
		}
		throw std::runtime_error(os.str());
	}
}

void Assert(bool b, const std::string& hint) // проверка на логическое соответствие
{
	AssertEqual(b, true, hint);
}

class TestRunner
{
private:
	int fail_count = 0; // счетчик упавших тестов
public:
	template <class TestFunc> void RunTest(TestFunc func, const std::string& test_name) // шаблонный запуск тестирующей функции
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
	~TestRunner() // проверка на верность тестов
	{
		if (fail_count > 0)
		{
			std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
			exit(1);
		}
	}
};
