#pragma once
// класс обертка для строки
class Counter
{
	char* m_pStr;
	size_t m_Owners;
	Counter* pNext;
	static Counter* Head;
	static size_t m_size;
	Counter(const char* const pStr);
	~Counter();
	static const Counter* checkAll(const char* const pStr);
	void addUser();
	void removeUser();
	friend class MyString;
};

//лучше выбирать один из 2 вариантов: сделать страж Head и зацикленный список. Либо Head на 1 элемет и не зацикленный список.
