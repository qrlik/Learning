#include "key_value.h"

const char* gender[] = { "FEMALE", "MALE", "NA" };
using namespace std;

// реализация класса Data
std::ostream& operator<<(ostream& os, const Data& data)
{
	os << "Gender: " << gender[static_cast<int>(data.m_gender)] <<
		"\nPhone: " << data.m_phone <<
		"\nSalary: " << data.m_salary << endl;
	return os;
}

// реализация класса Pair
Pair& Pair::operator=(const Data& other)
{
	m_data = other;
	return *this;
}

bool Pair::operator== (const char* k) const
{
	return m_name == k; // оператор == для MyString
}

std::ostream& operator<<(ostream& os, const Pair& pair)
{
	os << pair.m_name << pair.m_data << endl;
	return os;
}

// реализация класса BD
void BD::addCap()
{
	m_c += 5; // увеличение емкости на 5
	Pair* tmp = new Pair[m_c];
	if (m_p) // проверка на нулевой указатель
	{
		for (size_t i = 0; i < m_n; i++)
		{
			tmp[i] = move(m_p[i]);
		}
	}
	delete[] m_p;
	m_p = tmp;
}

BD::BD(const BD& other)
{
	m_c = other.m_c;
	if (m_c) // проверка на пустую базу
	{
		m_p = new Pair[m_c];
		m_n = other.m_n;
		for (size_t i = 0; i < m_n; i++)
		{
			m_p[i] = other.m_p[i];
		}
	}
	else
	{
		m_p = nullptr;
		m_n = 0;
	}
}

BD::BD(BD&& other)
{
	m_c = other.m_c;
	m_p = other.m_p;
	other.m_p = nullptr;
	m_n = other.m_n;
}

BD::~BD()
{
	delete[] m_p;
}

BD& BD::operator=(const BD& other)
{
	if (this != &other)
	{
		if (m_c < other.m_n) // если размера не хватает, перевыделяем память
		{
			delete[] m_p;
			m_c = other.m_n;
			m_p = new Pair[m_c];
		}
		m_n = other.m_n;
		for (size_t i = 0; i < m_n; i++)
		{
			m_p[i] = other.m_p[i];
		}
	}
	return *this;
}

BD& BD::operator=(BD&& other)
{
	if (this != &other)
	{
		delete[] m_p;
		m_p = other.m_p;
		m_c = other.m_c;
		m_n = other.m_n;
		other.m_p = nullptr;
		other.m_c = 0;
		other.m_n = 0;
	}
	return *this;
}

Pair& BD::operator[] (const char* str)
{
	for (size_t i = 0; i < m_n; i++)
	{
		if (m_p[i] == str)  // проверка на совпадение
		{
			return m_p[i];
		}
	}
	if (m_n == m_c) // проверка на заполнение памяти
	{
		addCap();
	}
	m_p[m_n].m_name = str; // добавление нового ключа
	m_n++; // размер + 1
	return m_p[m_n - 1];
}

void BD::remove(const char* str)
{
	for (size_t i = 0; i < m_n; i++)
	{
		if (m_p[i] == str)  // проверка на совпадение
		{
			if (i != m_n - 1) // проверка на конец базы
			{
				m_p[i] = move(m_p[m_n - 1]); // если элемент не в хвосте, хвост переносится на m_p[i]
			}
			m_n--; // если в хвосте просто уменьшаем размер
			break;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const BD& bd)
{
	for (size_t i = 0; i < bd.m_n; i++)
	{
		os << bd.m_p[i] << endl;
	}
	return os;
}
