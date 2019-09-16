#pragma once

class Point
{
	int m_x, m_y;
public:
	explicit Point(int x = 0, int y = 0) : m_x(x), m_y(y) {};
	const Point& operator+=(const Point& other);
	const Point& operator+=(const int& other);
	Point operator+(const Point& other) const;
	Point operator+(const int& other) const;
	const Point& operator+() const;
	const Point operator-() const;
	friend const Point& operator -= (Point& p1, const Point& p2);
	friend const Point operator - (const Point& p1, const Point& p2);
	friend const Point operator - (const Point& p1, const int& p2);
};

const Point& operator -= (Point& p1, const Point& p2);
const Point operator - (const Point& p1, const Point& p2);
const Point operator - (const Point& p1, const int& p2);
