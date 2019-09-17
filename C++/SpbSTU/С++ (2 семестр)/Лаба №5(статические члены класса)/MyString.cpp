#include "MyString.h"
#include <string.h>
#include <iostream>

MyString::MyString(const char* pStr)
{
	const Counter* result = nullptr;
	if (!pStr) // проверка на NULL
	{
		pStr = "Empty";
	}
	result = Counter::checkAll(pStr);

	if (result) // если уже есть такая строка
	{
		m_Counter = const_cast<Counter*>(result); // const Counter* -> Counter*
		m_Counter->addUser(); // добавление пользователя
	}
	else // если строки нет
	{
		m_Counter = new Counter(pStr);
	}
}

MyString::MyString(MyString&& other) : m_Counter(other.m_Counter)
{
	const Counter* result = Counter::checkAll("Empty");
	if (result)
	{
		other.m_Counter = const_cast<Counter*>(result);
		other.m_Counter->addUser(); // добавление пользователя
	}
	else
	{
		other.m_Counter = new Counter("Empty");
	}
}

MyString& MyString::operator=(const MyString& other)
{
	if (m_Counter != other.m_Counter) // в данном случае более общее условие, чем this != &other
	{
		m_Counter->removeUser();
		m_Counter = other.m_Counter;
		m_Counter->addUser();
	}
	return *this;
}

MyString& MyString::operator=(MyString&& other)
{
	if (m_Counter != other.m_Counter)
	{
		m_Counter->removeUser();
		m_Counter = other.m_Counter;
		MyString tmp = std::move(other); // создаем временный объект tmp, без затрат памяти(tmp и m_Counter ссылаются на один объект)
		// other направляем на "Empty"
	} // без delete происходит tmp->m_Counter->removeUser()
	return *this;
}

MyString::~MyString()
{
	m_Counter->removeUser();
}

void MyString::print() // печать всех элементов
{
	Counter* tmp = Counter::Head;
	std::cout << std::endl;
	for (size_t i = 0; i < Counter::m_size; i++) // печать всех элементов
	{
		std::cout << i + 1 << ". " << tmp->m_pStr << std::endl;
		tmp = tmp->pNext;
	}
}

void MyString::changeReg() // изменения регистра всех строк
{
	Counter* tmp = Counter::Head; // указатель на первую строку
	for (size_t i = 0; i < Counter::m_size; i++)
	{
		for (size_t j = 0; j < strlen(tmp->m_pStr); j++)
		{
			if ((tmp->m_pStr[j] >= 'a' && tmp->m_pStr[j] <= 'z') || (tmp->m_pStr[j] >= 'A' && tmp->m_pStr[j] <= 'Z'))
			{
				tmp->m_pStr[j] = tmp->m_pStr[j] ^ 0x20; // вычитаем или прибавляем 32 в зависимости от регистра
			}
		}
		tmp = tmp->pNext;
	}
}

void MyString::sort()
{
	Counter* pmin = Counter::Head; // указатель на минимум
	Counter* now = Counter::Head;
	Counter* srch = Counter::Head->pNext; // указатель для поиска минимума
	for (size_t i = 0; i < Counter::m_size - 1; i++) // -1 так как последний окажется на своем месте
	{
		for (size_t j = i + 1; j < Counter::m_size; j++) // ищем минимум
		{
			if (strcmp(pmin->m_pStr, srch->m_pStr) > 0)
			{
				pmin = srch;
			}
			srch = srch->pNext;
		}
		if (pmin != now) //перестановка элементов pmin и now если был найден минимум
		{
			Counter* tmp = Counter::Head;
			if (tmp != now)
			{
				while (tmp->pNext != now) // ищем элемент перед now , меняем указатель на now
				{
					tmp = tmp->pNext;
				}
				tmp->pNext = pmin;
			}
			
			tmp = now;
			while (tmp->pNext != pmin) // ищем элемент перед pmin, меняем указатель на pmin
			{
				tmp = tmp->pNext;
			}
			tmp->pNext = now;

			tmp = now->pNext; // меняем непосредственно указатели now и pmin
			now->pNext = pmin->pNext;
			pmin->pNext = tmp;
			if (i == 0) // проверка на 0 итерацию и изменение Head
			{
				Counter::Head = pmin;
			}
		}
		pmin = pmin->pNext; // изменение указателей перед следующей итерацией
		now = pmin;
		srch = now->pNext;
	}
}
