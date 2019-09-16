#pragma once
#include "mystring.h"
#include <iostream>

enum GENDER { FEMALE, MALE, NA };

class Data // класс данных
{
	GENDER m_gender;
	int m_phone;
	int m_salary;
public:
	Data(const GENDER& gender = NA, const int& phone = 911, const int& salary = 0) : m_gender(gender), m_phone(phone), m_salary(salary) {};
	friend std::ostream& operator<<(std::ostream& os, const Data& data);
	friend class Pair;
};

class Pair // класс пары ключ-данные
{
	Data m_data;
	MyString m_name;
	Pair() : m_name("empty"), m_data() {};
	Pair(const char* key, const Data& data) : m_name(key), m_data(data) {};
	bool operator== (const char* k) const;
public:
	Pair& operator=(const Data& other);
	friend std::ostream& operator<<(std::ostream& os, const Pair& pair);
	friend class BD;
};

class BD // класс базы данных
{
	Pair* m_p;
	size_t m_n;
	size_t m_c;
	void addCap();
public:
	BD() : m_p(nullptr), m_n(0), m_c(0) {};
	BD(const BD& other);
	BD(BD&& other);
	~BD();
	BD& operator=(const BD& other);
	BD& operator=(BD&& other);
	Pair& operator[](const char* str);
	void remove(const char* str);
	friend std::ostream& operator<<(std::ostream& os, const BD& bd);
};

std::ostream& operator<<(std::ostream& os, const Data& data);
std::ostream& operator<<(std::ostream& os, const Pair& pair);
std::ostream& operator<<(std::ostream& os, const BD& bd);
