// Темы:
// Перегрузка операторов.
// Встроенные объекты 

//#include "stdafx.h"	//если Вы используете предкомпиляцию заголовочных
					//файлов, раскомментируйте эту строчку
#include <tchar.h>
#include "mystring.h"
#include "point.h"
#include "key_value.h"
#define	  stop __asm nop

int _tmain(int argc, _TCHAR* argv[])
{
	//Задание 1.Перегрузка операторов.

	//Подключите к проекту файлы MyString.cpp и MyString.h
	//класса MyString, разработанного Вами на предыдущем занятии
	//9a. Перегрузите оператор присваивания для класса MyString
	//Проверьте корректность работы перегруженного оператора

	{
		MyString s1("AAA"), s2;
		s2=s1;
		s1="CCC";
		s2= MyString("tmp"); // move-конструктор
		s1=s1;
		stop
	}

	//Создайте класс Point, который будет содержать координаты
	//"точки". Объявите конструктор (конструкторы) класса

	//1b. "Перегрузите" оператор += так, чтобы при выполнении
	//операции типа pt2+=pt1; складывались
	//одноименные переменные объектов pt1 и pt2, а сумма
	//присваивалась соответствующим переменным объекта pt2 или
	// в случае  pt2+=1; обе переменные объекта pt2 увеличивались на
	// указанное значение
	//a) с помощью метода класса (оператор +=)
	//б) с помощью глобальной функции (оператор -=)
	//Проверьте корректность работы перегруженного оператора
	{
		Point pt1(1, 1);
		Point pt2(2, 2);
		pt2 += pt1;
		pt2 += 1;
		Point pt3(3, 3);
		pt2 += pt1 += pt3;
		pt3 -= pt2;
		pt1 += pt3 = pt2; // сначало pt3 = pt2, потом pt1 += pt3
		stop
	}

	//Задание 1c. Перегрузите оператор +/-
	//a)с помощью методов класса (оператор +)
	//б) с помощью  глобальных функций (оператор -)
	//так, чтобы любая из закомментированных строк
	//кода выполнялась корректно
	//Задание 1d. Перегрузите унарный оператор +/-
	{
		Point pt1(1, 1);
		Point pt2(2, 2);
		Point pt3;

		pt3 = pt1 + 5;
		//	pt3 = 2 + pt1;
		pt3 = pt1 + pt2;

		pt3 = pt1 - 5;
		pt3 = pt1 - pt2;
		pt3 = +pt1;
		stop
	}

	//Задание 1d. Перегрузите оператор << (вывода в поток) для
	//класса MyString таким образом, чтобы при выполнении приведенной строки
	//на экран было выведено:
	//contents:  "QWERTY"
	{
		MyString s("QWERTY"); //память
		std::cout << s << std::endl; // память добавляется отсюда.....

		//Задание 1е Перегрузите оператор + и += для класса MyString
		MyString s1("ABC"), s2("kkk");
		s1 += s;
		s2 = s + s1;
		stop
	}
	stop
	//Задание 2. Разработать "базу данных" о сотрудниках посредством ассоциативного
	//массива. Ключом является фамилия (ключ в задании уникален, поэтому нескольких Ивановых
	//в базе быть не может), а данными: пол, возраст, должность, зарплата...
	//Реализуйте:
	//добавление сотрудников в базу
	//исключение
	//вывод информации о конкретном сотруднике
	//вывод всей (или интересующей) информации о всех сотрудниках
	//Например:

	BD bd;	//создание пустой базы
	bd["Ivanov"] = Data(MALE, 900322, 15000);	//если Ivanov-а в базе еще нет, то
					//добавление, если уже есть, то замена его реквизитов
	std::cout << bd;	//вывод информации обо всех сотрудниках
	//А также:
	BD bdnew = bd;
	bdnew = bd;
	bdnew.remove("Ivanov");
	stop
	//А также можно вспомнить про семантику перемещения...

	return 0;
}//endmain

