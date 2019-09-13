#pragma once

class MyString
{
	char* m_pStr;	//строка-член класса
public:
	MyString(const char* const pStr = nullptr);
	MyString(const MyString& const other);
	MyString(MyString&& const other);
	~MyString();
	const char* const getString() const;
	void setString(const char* const pStr);
	MyString& operator=(const MyString& other);
	MyString& operator=(const char* const other);
	MyString& operator=(MyString&& other);
};

MyString connectString(const char* const str1, ...);
