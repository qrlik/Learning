#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T> ostream& operator << (ostream& os, const vector<T>& s) // шаблонный вывод вектора
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

template <class T> ostream& operator << (ostream& os, const set<T>& s) // шаблонный вывод множества
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

template <class K, class V> ostream& operator << (ostream& os, const map<K, V>& m) // шаблонный вывод карты
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

template<class T, class U> void AssertEqual(const T& t, const U& u, const string& hint = {}) // проверка на равенство объектов
{
	if (t != u)
	{
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty()) {
			os << " hint: " << hint;
		}
		throw runtime_error(os.str());
	}
}

void Assert(bool b, const string& hint) // проверка на логическое соответствие
{
	AssertEqual(b, true, hint);
}

class TestRunner
{
private:
	int fail_count = 0; // счетчик упавших тестов
public:
	template <class TestFunc> void RunTest(TestFunc func, const string& test_name) // шаблонный запуск тестирующей функции
	{
		try
		{
			func();
			cerr << test_name << " OK" << endl;
		}
		catch (exception & e)
		{
			++fail_count;
			cerr << test_name << " fail: " << e.what() << endl;
		}
		catch (...)
		{
			++fail_count;
			cerr << "Unknown exception caught" << endl;
		}
	}
	~TestRunner() // проверка на верность тестов
	{
		if (fail_count > 0)
		{
			cerr << fail_count << " unit tests failed. Terminate" << endl;
			exit(1);
		}
	}
};

void TestAll() // список всех тестов
{
	TestRunner runner;
}

int main() {
	TestAll();
	return 0;
}
