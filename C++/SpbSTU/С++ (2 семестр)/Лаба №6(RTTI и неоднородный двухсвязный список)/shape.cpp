#include "shape.h"
#include <cstdio>
#include <math.h>

//const char* colArr[] = { "RED", "GREEN", "BLUE" };

// реализация Shape
Shape::Shape(const COLOR& col)
{
	this->m_col = (static_cast<int>(col) >= 0 && static_cast<int>(col) <= 3) ? col : RED;
}

void Shape::setCol(const COLOR& col)
{
	this->m_col = col;
}

const COLOR& Shape::getCol() const
{
	return m_col;
}

Shape& Shape::operator=(const Shape& other)
{
	if (this != &other)
	{
		m_col = other.m_col;
	}
	return *this;
}

bool Shape::operator== (const Shape& rs) const
{
	return m_col == rs.m_col;
}

bool Shape::compareCol(const Shape& rhs)
{
	return static_cast<int>(m_col) > rhs.m_col;
}

bool Shape::compareSqr(const Shape& rhs)
{
	return square() > rhs.square();
}

bool Shape::compareDst(const Shape& rhs)
{
	return distance() > rhs.distance();
}

// реализиция Rect

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

Rect::Rect(const int& left, const int& right, const int& top, const int& bottom, const COLOR& col) : Shape(col), m_left(left), m_right(right), m_top(top), m_bottom(bottom)
{
	checkPoints();
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

Shape& Rect::operator=(const Shape& other)
{
	if (this != &other)
	{
		*this = static_cast<const Rect&>(other);
	}
	return *this;
}

bool Rect::operator== (const Shape& rs) const
{
	if (this->Shape::operator==(rs))
	{
		return (m_left == static_cast<const Rect&>(rs).m_left && m_right == static_cast<const Rect&>(rs).m_right && m_top ==
			static_cast<const Rect&>(rs).m_top && m_bottom == static_cast<const Rect&>(rs).m_bottom);
	}
	return false;
}

void Rect::Inflate(const int& x)
{
	m_left -= x;
	m_right += x;
	m_bottom -= x;
	m_top += x;
}

double Rect::square() const
{
	return (m_right - m_left) * (m_top - m_bottom);
}

double Rect::distance() const
{
	return sqrt(pow((m_left + (m_right - m_left) / 2), 2) + pow((m_bottom + (m_top - m_bottom) / 2), 2));
}

Shape* const Rect::make() const
{
	return new Rect(*this);
}

void Rect::print(std::ostream& os) const
{
	os << "Rect: (" << m_left << ", " << m_right << ", " << m_top << ", " << m_bottom << ", " << getCol() << ")";
}

// реализация Circle

void Circle::SetAll(const double& x, const double& y, const double& r)
{
	this->m_x = x;
	this->m_y = y;
	this->m_r = r;
}

void Circle::GetAll(double& x, double& y, double& r) const
{
	x = this->m_x;
	y = this->m_y;
	r = this->m_r;
}

Shape& Circle::operator=(const Shape& other)
{
	if (this != &other)
	{
		*this = static_cast<const Circle&>(other);
	}
	return *this;
}

bool Circle::operator== (const Shape& rs) const
{
	if (this->Shape::operator==(rs))
	{
		return (m_x == static_cast<const Circle&>(rs).m_x && m_y == static_cast<const Circle&>(rs).m_y && m_r == static_cast<const Circle&>(rs).m_r);
	}
	return false;
}

void Circle::Inflate(const int& x)
{
	m_r += x;
}

double Circle::square() const
{
	return 3.14 * m_r * m_r;
}

double Circle::distance() const
{
	return sqrt(m_x * m_x + m_y * m_y);
}

Shape* const Circle::make() const
{
	return new Circle(*this);
}

void Circle::print(std::ostream& os) const
{
	os << "Circle: (" << m_x << ", " << m_y << ", " << m_r << ", " << getCol() << ")";
}
