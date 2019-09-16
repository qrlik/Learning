enum COLOR { RED, GREEN, BLUE };

// класс фигур
class Shape {
	COLOR col;
public:
	Shape(const COLOR& col = RED);
	virtual ~Shape();
	void setCol(COLOR col);
	const COLOR& getCol() const;
	virtual void WhereAmI() const;
	virtual void Inflate(const int& x) = 0;
};

// класс-наследник Прямоугольник
class Rect:public Shape {
	int m_left, m_right, m_top, m_bottom;
public:
	Rect() : Shape(GREEN), m_left(0), m_right(0), m_top(0), m_bottom(0) {};
	Rect(const int& left, const int& right, const int& top, const int& bottom, const COLOR& col);
	virtual ~Rect();
	void checkPoints();
	void SetAll(const int& left, const int& right, const int& top, const int& bottom);
	void GetAll(int& left, int& right, int& top, int& bottom) const;
	virtual void WhereAmI() const;
	virtual void Inflate(const int& x);
};

 // класс-наследние Круг
class Circle :public Shape {
	double m_x, m_y, m_r;
public:
	Circle(const double& x = 0, const double& y = 0, const double& r = 0, const COLOR& col = BLUE) : Shape(col), m_x(x), m_y(y), m_r((r > 0) ? r : 0) {};
	Circle(const Rect& other);
	virtual ~Circle();
	void SetAll(const double& x, const double& y, const double& r);
	void GetAll(double& x, double& y, double& r) const;
	virtual void WhereAmI() const;
	virtual void Inflate(const int& x);
};
