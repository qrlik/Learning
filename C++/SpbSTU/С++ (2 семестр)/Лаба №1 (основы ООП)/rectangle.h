#pragma once

class Rect {
	int m_left, m_right, m_top, m_bottom;

public:
	Rect();
	Rect(int m_left, int m_right, int m_top, int m_bottom);
	Rect(const Rect& const other);
	~Rect();
	void checkPoints();
	void InflateRect(int i1, int i2, int i3, int i4);
	void InflateRect(int i1 = 1, int i2 = 1);
	void SetAll(int m_left, int m_right, int m_top, int m_bottom);
	void GetAll(int& m_left, int& m_right, int& m_top, int& m_bottom) const;
	void BoundingRect3(const Rect& r1, const Rect& r2);
};

Rect BoundingRect(Rect r1, Rect r2);
Rect BoundingRect2(const Rect& r1, const Rect& r2);
