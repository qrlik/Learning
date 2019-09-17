#include "circle.h"
// реализация круга
void Circle::SetAll(const Point& p, const int& r)
{
	m_p = p;
	m_r = r;
}

void Circle::GetAll(Point& p, int& r) const
{
	p = m_p;
	r = m_r;
}

bool Circle::operator==(const Circle& c) const
{
	if ((m_p.getX() == c.m_p.getX()) && (m_p.getY() == c.m_p.getY()) && (m_r == c.m_r))
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
	os << "Point: (" << p.getX() << ", " << p.getY() << ") ";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Circle& c)
{
	os << c.m_p << "Radius: " << c.m_r;
	return os;
}
