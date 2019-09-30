#include "rectangle.h"
#include <math.h>

Rect::Rect()
{
	m_left = 0;
	m_right = 0;
	m_top = 0;
	m_bottom = 0;
}

Rect::Rect(int m_left, int m_right, int m_top, int m_bottom)
{
	this->m_left = m_left;
	this->m_right = m_right;
	this->m_top = m_top;
	this->m_bottom = m_bottom;
	checkPoints();
}

Rect::Rect(const Rect& other)
{
	this->m_left = other.m_left;
	this->m_right = other.m_right;
	this->m_top = other.m_top;
	this->m_bottom = other.m_bottom;
}

Rect::~Rect() { }

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

void Rect::InflateRect(int i1, int i2, int i3, int i4)
{
	m_left -= i1;
	m_right += i2;
	m_top += i3;
	m_bottom -= i4;
	checkPoints();
}

void Rect::InflateRect(int i1, int i2)
{
	m_left -= i1;
	m_right += i1;
	m_bottom -= i2;
	m_top += i2;
	checkPoints();
}

void Rect::SetAll(int m_left, int m_right, int m_top, int m_bottom)
{
	this->m_left = m_left;
	this->m_right = m_right;
	this->m_top = m_top;
	this->m_bottom = m_bottom;
	checkPoints();
}

void Rect::GetAll(int& m_left, int& m_right, int& m_top, int& m_bottom) const
{
	m_left = this->m_left;
	m_right = this->m_right;
	m_top = this->m_top;
	m_bottom = this->m_bottom;
}

double Rect::getDistance() const
{
	return sqrt(pow((m_left + (m_right - m_left) / 2), 2) + pow((m_bottom + (m_top - m_bottom) / 2), 2));
}

Rect BoundingRect(Rect r1, Rect r2)
{
	int r1left, r1right, r1top, r1bottom;
	r1.GetAll(r1left, r1right, r1top, r1bottom);
	int r2left, r2right, r2top, r2bottom;
	r2.GetAll(r2left, r2right, r2top, r2bottom);
	int r3left, r3right, r3top, r3bottom;
	r3left = (r1left < r2left) ? r1left : r2left;
	r3right = (r1right < r2right) ? r2right : r1right;
	r3top = (r1top < r2top) ? r2top : r1top;
	r3bottom = (r1bottom < r2bottom) ? r1bottom : r2bottom;
	Rect r3(r3left, r3right, r3top, r3bottom);
	return r3;
}

Rect BoundingRect2(Rect& r1, Rect& r2)
{
	int r1left, r1right, r1top, r1bottom;
	r1.GetAll(r1left, r1right, r1top, r1bottom);
	int r2left, r2right, r2top, r2bottom;
	r2.GetAll(r2left, r2right, r2top, r2bottom);
	int r3left, r3right, r3top, r3bottom;
	r3left = (r1left < r2left) ? r1left : r2left;
	r3right = (r1right < r2right) ? r2right : r1right;
	r3top = (r1top < r2top) ? r2top : r1top;
	r3bottom = (r1bottom < r2bottom) ? r1bottom : r2bottom;
	Rect r3(r3left, r3right, r3top, r3bottom);
	return r3;
}

void Rect::BoundingRect3(Rect& r1, Rect& r2)
{
	m_left = (r1.m_left < r2.m_left) ? r1.m_left : r2.m_left;
	m_right = (r1.m_right < r2.m_right) ? r2.m_right : r1.m_right;
	m_top = (r1.m_top < r2.m_top) ? r2.m_top : r1.m_top;
	m_bottom = (r1.m_bottom < r2.m_bottom) ? r1.m_bottom : r2.m_bottom;
}

std::ostream& operator<<(std::ostream& os, const Rect& mStr)
{
	os << mStr.m_left << " " << mStr.m_right << " " << mStr.m_top << " " << mStr.m_bottom;
	return os;
}
