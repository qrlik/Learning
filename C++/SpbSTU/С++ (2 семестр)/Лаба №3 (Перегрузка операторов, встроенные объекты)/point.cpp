#include "point.h"

const Point& Point::operator+=(const Point& other)
{
	m_x = m_x + other.m_x;
	m_y = m_y + other.m_y;
	return *this;
}

const Point& Point::operator+=(const int& other)
{
	m_x = m_x + other;
	m_y = m_y + other;
	return *this;
}

Point Point::operator+(const Point& other) const
{
	return Point(m_x + other.m_x, m_y + other.m_y);
}

Point Point::operator+(const int& other) const
{
	return Point(m_x + other, m_y + other);
}

const Point& Point::operator+() const
{
	return *this;
}

const Point Point::operator-() const // const запрещает выражения типа -point1 = point2
{
	return Point(-m_x, -m_y);
}

const Point operator - (const Point& p1, const Point& p2) // const запрещает выражения типа Point1 - Point2 = Point3
{
	return Point(p1.m_x - p2.m_x, p1.m_x - p2.m_y);
}

const Point operator - (const Point& p1, const int& p2)
{
	return Point(p1.m_x - p2, p1.m_x - p2);
}

const Point& operator -= (Point& p1, const Point& p2)
{
	p1.m_x = p1.m_x - p2.m_x;
	p1.m_y = p1.m_y - p2.m_y;
	return p1;
}
