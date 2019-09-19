#define _CRT_SECURE_NO_WARNINGS
#include "mystring.h"
#include <string>

using namespace std;

// Определение конструктора.
MyString::MyString(const char* const pStr)
{
	if (pStr) // проверка на Null
	{
		m_pStr = new char[strlen(pStr) + 1];
		strcpy(m_pStr, pStr);
	}
	else
	{
		m_pStr = nullptr;
	}
}

MyString::MyString(const MyString& other)
{
	if (other.m_pStr) // проверка на Null
	{
		m_pStr = new char[strlen(other.m_pStr) + 1];
		strcpy(m_pStr, other.m_pStr);
	}
	else
	{
		m_pStr = nullptr;
	}
}

MyString::MyString(MyString&& other)
{
	m_pStr = other.m_pStr;
	other.m_pStr = nullptr;
}

// Определение деструктора.

MyString::~MyString()
{
	delete[] m_pStr;
}

const char* const MyString::getString() const
{
	if (!this)
	{
		return "No string";
	}
	if (!m_pStr)
	{
		return "No string";
	}
	else
	{
		return m_pStr;
	}
}

void MyString::setString(const char* const pStr)
{
	if (!pStr)
	{
		delete[] m_pStr;
		m_pStr = nullptr;
	}
	else
	{
		if (!m_pStr) // проверка на пустой указатель
		{
			m_pStr = new char[strlen(pStr) + 1];
			strcpy(m_pStr, pStr);
		}
		else if (strlen(m_pStr) >= strlen(pStr)) // проверка на достаточность уже выделенной памяти
		{
			strcpy(m_pStr, pStr);
		}
		else // если памяти не хватает, перевыделяем
		{
			delete[] m_pStr;
			m_pStr = new char[strlen(pStr) + 1];
			strcpy(m_pStr, pStr);
		}
	}
}

const MyString connectString(const char* const str1, ...)
{
	const char*const* p = &str1; // указатель на первую переменную
	size_t size = 0;
	while (*p != nullptr) // расчет необходимого размера
	{
		size += strlen(*p);
		p++;
	}
	char* const result = new char[size + 1]; // выделение памяти
	result[0] = '\0';
	p = &str1; // возвращение указателя на первую переменную
	while (*p != nullptr) // конкатенация строк
	{
		strcat(result, *p);
		p++;
	}
	MyString finalString(result); // создание объекта, чтобы освободить память
	delete[] result;
	return finalString;
}

MyString& MyString::operator=(const MyString& other)
{
	if (this != &other)
	{
		if (!other.m_pStr) // проверка на пустой указатель
		{
			delete[] m_pStr;
			m_pStr = nullptr;
		}
		else
		{
			if (!m_pStr) // проверка на пустой указатель
			{
				m_pStr = new char[strlen(other.m_pStr) + 1];
				strcpy(m_pStr, other.m_pStr);
			}
			else if (strlen(m_pStr) >= strlen(other.m_pStr)) // проверка на достаточность уже выделенной памяти
			{
				strcpy(m_pStr, other.m_pStr);
			}
			else // если памяти не хватает, перевыделяем
			{
				delete[] m_pStr;
				m_pStr = new char[strlen(other.m_pStr) + 1];
				strcpy(m_pStr, other.m_pStr);
			}
		}
	}
	return *this;
}

MyString& MyString::operator=(const char* const other)
{
	MyString tmp(other); // создаем временный объект на основе конструктора
	*this = tmp; // вызываем конструктор копирования
	return *this; // возвращаем обновленный объект
}

MyString& MyString::operator=(MyString&& other)
{
	if (this != &other)
	{
		delete[] m_pStr;
		m_pStr = other.m_pStr;
		other.m_pStr = nullptr;
	}
	return *this;
}

const MyString& MyString::operator+=(const MyString& other)
{
	char* pStr = new char[strlen(m_pStr) + strlen(other.m_pStr) + 1];
	pStr[0] = '\0';
	strcat(pStr, m_pStr);
	delete[] m_pStr;
	strcat(pStr, other.m_pStr);
	m_pStr = pStr;
	return *this;
}

const MyString operator+(const MyString& str1, const MyString& str2)
{
	char* pStr = new char[strlen(str1.getString()) + strlen(str2.getString()) + 1]; // выделяем память под сумму строк
	pStr[0] = '\0'; // для корректной работы strcat
	strcat(pStr, str1.getString()); // вносим строку str1
	MyString tmp(strcat(pStr, str2.getString())); // вносим строку str2 и создаем временный объект
	delete[] pStr; // удаляем буфферную строку
	return move(tmp); // указываем компилятору использовать move-оператор+
}

bool MyString::operator==(const char* other) const
{
	return !strcmp(getString(), other);
}

ostream& operator<<(ostream& os, const MyString& mStr)
{
	os << "Name: " << mStr.getString() << endl;
	return os;
}
