#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
/*
Вам надо написать программу, которая считывает из стандартного ввода одну строку в формате ДРОБЬ_1 операция ДРОБЬ_2.
ДРОБЬ_1 и ДРОБЬ_2 имеют формат X/Y, где X — целое, а Y — целое неотрицательное число.
операция — это один из символов '+', '-', '*', '/'.

Если ДРОБЬ_1 или ДРОБЬ_2 является некорректной обыкновенной дробью, ваша программа должна вывести в стандартный вывод сообщение "Invalid argument".
Если считанная операция — это деление на ноль, выведите в стандартный вывод сообщение "Division by zero". В противном случае выведите результат операции
*/

int greatest_common_factor(int x, int y) noexcept // поиск наибольшего общего делителя
{
	x = abs(x);
	y = abs(y);
	while (x != 0 && y != 0)
	{
		(x > y) ? (x = x % y) : (y = y % x);
	}
	return x + y;
}

class Rational // класс для работы с дробями
{
private:
	int m_p; // числитель
	int m_q; // знаменатель
public:
	Rational(int p = 0, int q = 1) throw(invalid_argument&) : m_p(p), m_q(q)
	{
		if (m_q == 0) // если знаменатель ноль
		{
			throw invalid_argument("0");
		}
		else if (m_p == 0) // если числитель ноль
		{
			m_q = 1;
		}
		else
		{ // сокращаем дробь
			m_p /= greatest_common_factor(p, q);
			m_q /= greatest_common_factor(p, q);
			if (m_q < 0) // если знаменатель отрицательный
			{ // меняем знак числителя
				m_p = -m_p;
				m_q = -m_q;
			}
		}
	}
	int Numerator() const noexcept { return m_p; }
	int Denominator() const noexcept { return m_q; }
	bool operator==(const Rational& rhs) const noexcept
	{
		return (m_p == rhs.m_p && m_q == rhs.m_q);
	}
	bool operator<(const Rational& rhs) const noexcept
	{
		int gcf = greatest_common_factor(m_q, rhs.m_q);
		return m_p * (rhs.m_q / gcf) < rhs.m_p * (m_q / gcf); // используя НОД сравниваем числители дробей
	}
	Rational operator+(const Rational& rhs) const noexcept
	{
		return Rational((m_p * rhs.m_q) + (rhs.m_p * m_q), m_q * rhs.m_q);
	}
	Rational operator-(const Rational& rhs) const noexcept
	{
		return Rational((m_p * rhs.m_q) - (rhs.m_p * m_q), m_q * rhs.m_q);
	}
	Rational operator*(const Rational& rhs) const noexcept
	{
		return Rational(m_p * rhs.m_p, m_q * rhs.m_q);
	}
	Rational operator/(const Rational& rhs) const throw (domain_error&)
	{
		if (rhs.m_p == 0)
		{
			throw domain_error("0");
		}
		return Rational(m_p * rhs.m_q, m_q * rhs.m_p);
	}
	friend istream& operator>>(istream& is, Rational& obj);
};

ostream& operator<<(ostream& os, const Rational& obj)
{
	os << obj.Numerator() << '/' << obj.Denominator();
	return os;
}

istream& operator>>(istream& is, Rational& obj)
{
	if (is.good()) // для 4 части почему-то нужен if(is.tellg() != -1) либо...
	{
		int p, q;
		is >> p;
		is.ignore(1);
		is >> q; // ... либо перенести if(is) сюда
		obj = Rational (p,q);
	}
	return is;
}

int main() 
{
	try
	{
		Rational lhs, rhs;
		char command;
		cin >> lhs >> command >> rhs; // считываем дроби и операцию
		switch (command)
		{
		case('+'):
			cout << lhs + rhs << endl;
			break;
		case('-'):
			cout << lhs - rhs << endl;
			break;
		case('*'):
			cout << lhs * rhs << endl;
			break;
		case('/'):
			cout << lhs / rhs << endl;
			break;
		default:
			cout << "wrong command!" << endl;
		}
	}
	catch (invalid_argument&) 
	{
		cout << "Invalid argument" << endl;
	}
	catch (domain_error&)
	{
		cout << "Division by zero" << endl;
	}
	return 0;
}
