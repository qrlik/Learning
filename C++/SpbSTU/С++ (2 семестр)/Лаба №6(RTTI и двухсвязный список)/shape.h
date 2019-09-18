#pragma once
#include <iostream>

enum COLOR { RED, GREEN, BLUE };

// класс фигур
class Shape {
	COLOR m_col;
protected:
	Shape(const COLOR& col = RED);
	void setCol(const COLOR& col);
	virtual Shape* const make() const = 0; // создание нового объекта
public:
	const COLOR& getCol() const;
	virtual Shape& operator=(const Shape& other);
	virtual bool operator== (const Shape& rs) const;
	bool compareCol(const Shape& rhs);
	bool compareSqr(const Shape& rhs);
	bool compareDst(const Shape& rhs);
	virtual void Inflate(const int& x) = 0;
	virtual void print(std::ostream& os) const = 0;
	virtual double square() const = 0; // рассчет площади фигуры
	virtual double distance() const = 0; // удаление от начала координат
	friend class Node;
	friend class List;
};

// класс-наследник Прямоугольник
class Rect :public Shape {
	int m_left, m_right, m_top, m_bottom; // virtual private члены будут вызываться через vtab класса Shape
	void checkPoints();
	void SetAll(const int& left, const int& right, const int& top, const int& bottom);
	virtual void Inflate(const int& x);
	virtual Shape* const make() const;
public:
	Rect() : Shape(GREEN), m_left(0), m_right(0), m_top(0), m_bottom(0) {}
	Rect(const int& left, const int& right, const int& top, const int& bottom, const COLOR& col);
	void GetAll(int& left, int& right, int& top, int& bottom) const;
	virtual Shape& operator=(const Shape& other); //
	virtual bool operator== (const Shape& ps) const;
	virtual double square() const;
	virtual double distance() const;
	virtual void print(std::ostream& os) const;
};

// класс-наследние Круг
class Circle :public Shape {
	double m_x, m_y, m_r;
	void SetAll(const double& x, const double& y, const double& r);
	virtual void Inflate(const int& x);
	virtual Shape* const make() const;
public:
	Circle(double x = 0, double y = 0, double r = 0, COLOR col = BLUE) : Shape(col), m_x(x), m_y(y), m_r((r >= 0) ? r : 0) {}
	void GetAll(double& x, double& y, double& r) const;
	virtual Shape& operator=(const Shape& other);
	virtual bool operator== (const Shape& ps) const;
	virtual double square() const;
	virtual double distance() const;
	virtual void print(std::ostream& os) const;
};
