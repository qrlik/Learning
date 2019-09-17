#define _CRT_SECURE_NO_WARNINGS
#include "Counter.h"
#include <string.h>
//реализация класса обертки Counter
Counter* Counter::Head = nullptr;
size_t Counter::m_size = 0;

Counter::Counter(const char* const pStr): m_Owners(1)
{
	m_pStr = new char[strlen(pStr) + 1]; // проверка на NULL в MyString
	strcpy(m_pStr, pStr); // выделение памяти и копирование
	pNext = (Head) ? Head : nullptr; // если список не пустой вставляем в начало, иначе pNext пустой
	Head = this;
	m_size++;
}

Counter::~Counter()
{
	if (Head == this)
	{
		Head = pNext; // перенаправляем на следующий
	}
	else
	{
		Counter* tmp = Head;
		while (tmp->pNext != this) // поиск предыдущего элемента
		{
			tmp = tmp->pNext;
		}
		tmp->pNext = pNext; // перекидываем указатель
	}
	m_size--; // уменьшаем размер списка
	delete[] m_pStr; // освобождаем память
}

const Counter* Counter::checkAll(const char* const pStr)// проверка списка на наличие строки
{
	Counter* srch = Head;
	for (size_t i = 0; i < m_size; i++)
	{
		if (!strcmp(srch->m_pStr, pStr))
		{
			return srch; // строка найдена
		}
		srch = srch->pNext;
	}
	return nullptr; // строка не найдена
}

void Counter::addUser()
{
	m_Owners++;
}

void Counter::removeUser()
{
	m_Owners--;
	if (!m_Owners) // нет владельцев -> удаление
	{
		delete this;
	}
}
