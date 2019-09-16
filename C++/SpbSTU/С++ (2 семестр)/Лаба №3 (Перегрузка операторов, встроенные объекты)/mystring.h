#pragma once
#include <iostream>

class MyString
{
	char* m_pStr;	//строка-член класса
public:
	MyString(const char* const pStr = nullptr);
	MyString(const MyString& other);
	MyString(MyString&& other);
	~MyString();
	const char* const getString() const;
	void setString(const char* const pStr);
	MyString& operator=(const MyString& other);
	MyString& operator=(const char* const other);
	MyString& operator=(MyString&& other);
	const MyString& operator+=(const MyString& other);
	bool operator==(const char* other) const;
};

const MyString operator+(const MyString& str1, const MyString& str2);
const MyString connectString(const char* const str1, ...);
std::ostream& operator<<(std::ostream& os, const MyString& mStr);
