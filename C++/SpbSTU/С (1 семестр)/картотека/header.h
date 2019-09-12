#pragma once
#include <iostream>

enum CATEGORY { History, Fantasy, Thriller, Romance }; // категории книг
//enum SORT { name, author, price, year, genre };

struct BOOK
{
	char name[20];
	char author[20];
	double price;
	unsigned int year;
	CATEGORY genre;
};

struct KARTOTEKA
{
	BOOK** pK;
	size_t m_n, m_cap;
};

void addBOOK(KARTOTEKA* const pkart); // добавление книги
void printKART(KARTOTEKA* const pkart); // печать всей картотеки
void remBOOK(KARTOTEKA* const pkart);
void remKART(KARTOTEKA& pkart); // освобождение памяти
void capKART(KARTOTEKA* const pkart); // увеличение картотеки

void fileIN(KARTOTEKA* const pkart); // считывание из файла
void fileOUT(KARTOTEKA* const pkart); // запись в файл

bool cmpName(const BOOK* const b1, const BOOK* const b2); // функции сравнения
bool cmpAuthor(const BOOK* const b1, const BOOK* const b2);
bool cmpPrice(const BOOK* const b1, const BOOK* const b2);
bool cmpYear(const BOOK* const b1, const BOOK* const b2);
bool cmpGenre(const BOOK* const b1, const BOOK* const b2);

void sortKART(KARTOTEKA* const pkart); // сортировка картотеки

int Menu(); // функция Меню

void(*menuAr[]) (KARTOTEKA* const); // массив указателей на функции. ВОПРОС С const для printKART и fileOUT
