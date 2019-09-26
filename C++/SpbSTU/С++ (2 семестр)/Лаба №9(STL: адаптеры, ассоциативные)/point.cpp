#include "point.h"
#include <string>
#include <cmath>

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Point::getX() const
{
	return x;
}

int Point::getY() const
{
	return y;
}

Point& Point::operator+=(const Point& other)
{
	x = x + other.x;
	y = y + other.y;
	return *this;
}

Point& Point::operator+=(int other)
{
	x = x + other;
	y = y + other;
	return *this;
}

Point Point::operator+(const Point& other) const
{
	return Point(x + other.x, y + other.y);
}

Point Point::operator+(int other) const
{
	return Point(x + other, y + other);
}

const Point& Point::operator+() const
{
	return *this;
}

Point Point::operator-() const
{
	return Point(-x, -y);
}

bool Point::operator<(const Point& other) const
{
	return (sqrt(pow(x, 2) + pow(y, 2)) < sqrt(pow(other.x, 2) + pow(other.y, 2)));
}

bool Point::operator==(const Point& other) const
{
	return (x == other.x) && (y == other.y);
}

bool IfNeg(const Point& p)
{
	return (p.getX() < 0) || (p.getY() < 0);
}

Point operator - (const Point& p1, const Point& p2)
{
	return Point(p1.x - p2.x, p1.x - p2.y);
}

Point operator - (const Point& p1, int p2)
{
	return Point(p1.x - p2, p1.x - p2);
}

Point& operator -= (Point& p1, const Point& p2)
{
	p1.x = p1.x - p2.x;
	p1.y = p1.y - p2.y;
	return p1;
}

std::ostream& operator<<(std::ostream& os, const Point& mP)
{
	os << mP.getX() << " " << mP.getY();
	return os;
}
