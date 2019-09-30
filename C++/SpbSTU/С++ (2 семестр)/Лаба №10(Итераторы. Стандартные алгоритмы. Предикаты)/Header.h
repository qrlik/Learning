#pragma once
using namespace std;
#include "rectangle.h"

template<typename T> void print_value(const T& v)
{
	cout << "Value: " << v << endl;
}

class plus_value
{
	int m_x;
public:
	plus_value(int x): m_x(x) {}
	template <typename T> void operator()(T& v) { v += m_x; }
};

class point_range
{
	int m_x, m_y;
public:
	point_range(int x, int y): m_x(x), m_y(y) {}
	bool operator()(const Point& p)
	{
		return (p.getX() >= m_x && p.getX() <= m_y) && (p.getY() >= m_x && p.getY() <= m_y);
	}
};

bool cmp_distance(Rect& r1, Rect& r2)
{
	return r1.getDistance() < r2.getDistance();
}

string lower_string(string s)
{
	transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}
