#pragma once
#include <iostream>

class Point
{
	int x, y;
public:
	explicit Point(int x = 0, int y = 0);
	int getX() const;
	int getY() const;
	Point& operator+=(const Point& other);
	Point& operator+=(int other);
	Point operator+(const Point& other) const;
	Point operator+(int other) const;
	const Point& operator+() const;
	Point operator-() const;
	friend Point& operator -= (Point& p1, const Point& p2);
	friend Point operator - (const Point& p1, const Point& p2);
	friend Point operator - (const Point& p1, int p2);
	bool operator<(const Point& other) const;
	bool operator==(const Point& other) const;
};

bool IfNeg(const Point& p);
Point& operator -= (Point& p1, const Point& p2);
Point operator - (const Point& p1, const Point& p2);
Point operator - (const Point& p1, int p2);
std::ostream& operator<<(std::ostream& os, const Point& mP);
