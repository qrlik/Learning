#include "shape.h"
#include <cstdio>
#include <math.h>

		// реализация Shape
Shape::Shape(const COLOR& col)
{
	this->col = (static_cast<int>(col) >= 0 && static_cast<int>(col) <= 3) ? col : RED;
}

Shape::~Shape()
{
	printf("Now I am in Shape's destructor!\n");
}

void Shape::setCol(COLOR col)
{
	this->col = col;
}

const COLOR& Shape::getCol() const
{
	return col;
}

void Shape::WhereAmI() const
{
	printf("Now I am in class Shape\n");
}

		// реализиция Rect
Rect::Rect(const int& left, const int& right, const int& top, const int& bottom, const COLOR& col): Shape(col), m_left(left), m_right(right), m_top(top), m_bottom(bottom)
{
	checkPoints();
}

Rect::~Rect()
{
	printf("Now I am in Rect's destructor!\n");
} 

void Rect::checkPoints()
{
	if (m_left > m_right)
	{
		int tmp = m_right;
		m_right = m_left;
		m_left = tmp;
	}
	if (m_bottom > m_top)
	{
		int tmp = m_top;
		m_top = m_bottom;
		m_bottom = tmp;
	}
}

void Rect::SetAll(const int& left, const int& right, const int& top, const int& bottom)
{
	this->m_left = left;
	this->m_right = right;
	this->m_top = top;
	this->m_bottom = bottom;
	checkPoints();
}

void Rect::GetAll(int& left, int& right, int& top, int& bottom) const
{
	left = this->m_left;
	right = this->m_right;
	top = this->m_top;
	bottom = this->m_bottom;
}

void Rect::WhereAmI() const
{
	printf("Now I am in class Rect\n");
}

void Rect::Inflate(const int& x)
{
	m_left -= x;
	m_right += x;
	m_bottom -= x;
	m_top += x;
}

			// реализация Circle
Circle::Circle(const Rect& other): Shape(other)
{
	int left, right, top, bottom;
	other.GetAll(left, right, top, bottom);
	m_x = (static_cast<double>(right) - left) / 2;
	m_y = (static_cast<double>(top) - bottom) / 2;
	m_r = sqrt(pow(static_cast<double>(right) - left, 2) + pow(static_cast<double>(top) - bottom, 2)) / 2;
}

Circle::~Circle()
{
	printf("Now I am in Circle's destructor!\n");
}

void Circle::SetAll(const double& x, const double& y, const double& r)
{
	this->m_x = x;
	this->m_y = y;
	this->m_r = (r > 0) ? r : 0;
}

void Circle::GetAll(double& x, double& y, double& r) const
{
	x = this->m_x;
	y = this->m_y;
	r = this->m_r;
}

void Circle::WhereAmI() const
{
	printf("Now I am in class Circle\n");
}

void Circle::Inflate(const int& x)
{
	m_r += x;
}
