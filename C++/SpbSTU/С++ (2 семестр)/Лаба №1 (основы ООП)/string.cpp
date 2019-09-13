#define _CRT_SECURE_NO_WARNINGS
#include "string.h"
#include <string>
using namespace std;

// Определение конструктора.
MyString::MyString(const char* const pStr)
{
	if (pStr) // проверка на значение по умолчанию
	{
		m_pStr = new char[strlen(pStr) + 1];
		strcpy(m_pStr, pStr);
	}
	else
	{
		m_pStr = nullptr;
	}
}

MyString::MyString(const MyString& const other)
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

MyString::MyString(MyString&& const other)
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
	if (!this) // защита от MyString* p = nullptr; p->getString()
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

MyString connectString(const char* const str1, ...)
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
	if (other.m_pStr) // проверка на значение по умолчанию
	{
		m_pStr = new char[strlen(other.m_pStr) + 1];
		strcpy(m_pStr, other.m_pStr);
	}
	else
	{
		m_pStr = nullptr;
	}
	return *this;
}

MyString& MyString::operator=(const char* const other)
{
	if (other) // проверка на значение по умолчанию
	{
		m_pStr = new char[strlen(other) + 1];
		strcpy(m_pStr, other);
	}
	else
	{
		m_pStr = nullptr;
	}
	return *this;
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
