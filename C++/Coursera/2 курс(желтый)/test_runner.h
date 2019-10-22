#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>

void Assert(bool b, const std::string& hint); // проверка на логическое соответствие

class TestRunner
{
private:
	int fail_count = 0; // счетчик упавших тестов
public:
	template <class TestFunc> void RunTest(TestFunc func, const std::string& test_name); // шаблонный запуск тестирующей функции
	~TestRunner(); // проверка на верность тестов
};

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
