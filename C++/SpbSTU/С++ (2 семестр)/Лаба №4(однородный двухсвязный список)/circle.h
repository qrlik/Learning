#pragma once
#include <iostream>
// интерфейс точки
class Point
{
	int m_x, m_y;
public:
	explicit Point(const int& x = 0, const int& y = 0) : m_x(x), m_y(y) {}
	const int& getX() const { return m_x; }
	const int& getY() const { return m_y; }
	void setX(const int& x) { m_x = x; }
	void setY(const int& y) { m_y = y; }
};
// интерфейс круга
class Circle
{
	Point m_p;
	int m_r;
public:
	Circle() : m_p(), m_r(0) {}
	Circle(const Point& p, const int& r = 0) : m_p(p), m_r((r > 0) ? r : 0) {}
	Circle(const int& x, const int& y, const int& r = 0) : m_p(x, y), m_r((r > 0) ? r : 0) {}
	void SetAll(const Point& p, const int& r);
	void GetAll(Point& p, int& r) const;
	const int& GetR() const { return m_r; }
	void Inflate(const int& x) { m_r += x; }
	bool operator==(const Circle& c) const;
	friend std::ostream& operator<<(std::ostream& os, const Circle& c);
};

std::ostream& operator<<(std::ostream& os, const Point& p);
std::ostream& operator<<(std::ostream& os, const Circle& c);
