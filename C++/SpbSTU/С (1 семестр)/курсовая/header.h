/* 11 вариант. Задание:
Посредством структур реализуйте двухсвязный список для хранения прямоугольников.Обеспечьте следующую функциональность :
А) добавить прямоугольник в список
Б) изъять из списка все прямоугольники, площадь которых совпадает с указанным
В) сохранять данные в файле
Г) считывать данные из файла
Д) сортировать список согласно заданному критерию */

#pragma once
#include <iostream>

struct rectangle // структура прямоугольника (звена списка)
{
	double a;
	double b;
	double S;
	rectangle* next;
	rectangle* prev;
};

struct list // структура списка
{
	rectangle* head;
	rectangle* tail;
};

void printLIST(list* const list); // печать всего списка
void deleteRectangle(rectangle* const rectangle, list* const list); // удаление 1 узла
void addRectangle(list* const list); // добавление прямоугольника
void remRectangle(list* const list); // удаление на выбор (один или все узлы)
void remSquare(list* const list); // удаление по введенной площади

bool cmpA(const rectangle* const b1, const rectangle* const b2); // функции сравнения
bool cmpB(const rectangle* const b1, const rectangle* const b2);
bool cmpS(const rectangle* const b1, const rectangle* const b2);
void sortLIST(list* const list); // сортировка списка

void fileOUT(list* const list); // запись в файл
void fileIN(list* const list); // считывание из файла

void remList(list& list); // "деконструктор"

void(*menuRec[]) (list* const); // массив указателей на функции меню

int Menu(); // меню
