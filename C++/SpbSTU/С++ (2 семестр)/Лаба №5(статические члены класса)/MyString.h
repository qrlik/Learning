#pragma once
#include "Counter.h"

class MyString
{
	Counter* m_Counter;
public:
	MyString(const char* pStr = nullptr);
	MyString(const MyString& other) : m_Counter(other.m_Counter) { m_Counter->addUser(); }
	MyString(MyString&& other);
	MyString& operator=(const MyString& other);
	MyString& operator=(MyString&& other);
	~MyString();
	static void print();
	static void changeReg();
	static void sort();
};

// лучше сделать конструктор по умолчанию с nullptr, а Emptry убрать
