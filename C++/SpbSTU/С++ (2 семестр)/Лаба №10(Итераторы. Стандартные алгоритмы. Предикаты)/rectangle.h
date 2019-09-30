#pragma once
#include <iostream>

class Rect {
	int m_left, m_right, m_top, m_bottom;

public:
	Rect();
	Rect(int m_left, int m_right, int m_top, int m_bottom);
	Rect(const Rect& other);
	~Rect();
	void checkPoints();
	void InflateRect(int i1, int i2, int i3, int i4);
	void InflateRect(int i1 = 1, int i2 = 1);
	void SetAll(int m_left, int m_right, int m_top, int m_bottom);
	void GetAll(int& m_left, int& m_right, int& m_top, int& m_bottom) const;
	double getDistance() const;
	void BoundingRect3(Rect& r1, Rect& r2);
	friend std::ostream& operator<<(std::ostream& os, const Rect& mStr);
};

Rect BoundingRect(Rect r1, Rect r2);
Rect BoundingRect2(Rect& r1, Rect& r2);

std::ostream& operator<<(std::ostream& os, const Rect& mStr);
