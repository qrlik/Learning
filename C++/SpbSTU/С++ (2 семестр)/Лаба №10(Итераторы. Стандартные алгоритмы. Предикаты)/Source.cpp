// Iter_Alg.cpp : Defines the entry point for the console application.
//Итераторы. Стандартные алгоритмы. Предикаты.

#include <iterator>
#include <typeinfo>
#include <string>
#include <clocale>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <tchar.h>
#include "point.h"
#include "Header.h"

using namespace std;
#define	  stop __asm nop


int _tmain(int argc, _TCHAR* argv[])
{

	///////////////////////////////////////////////////////////////////

	//Задание 1. Итераторы

	//Реверсивные итераторы. Сформируйте set<Point>. Подумайте, что
	//нужно перегрузить в классе Point. Создайте вектор, элементы которого 
	//являются копиями элементов set, но упорядочены по убыванию
	set<Point> s1 = { Point(0,0), Point(1,1), Point(2,3) };
	vector<Point> v1(s1.rbegin(), s1.rend());
	stop

	//Потоковые итераторы. С помощью ostream_iterator выведите содержимое
	//vector и set из предыдущего задания на экран.
	
	copy(s1.begin(), s1.end(), ostream_iterator<Point>(cout, "|"));
	cout << endl; // 0 0|1 1|2 3|
	copy(v1.begin(), v1.end(), ostream_iterator<Point>(cout, "|"));
	cout << endl; // 2 3|1 1|0 0|
	stop

	//Итераторы вставки. С помощью возвращаемых функциями:
	//back_inserter() не адаптеры, не ассоциативный
	//front_inserter() не вектор, не ассоциативный, не адаптер
	//inserter() с любым
	//итераторов вставки добавьте элементы в любой из созданных контейнеров. Подумайте:
	//какие из итераторов вставки можно использовать с каждым контейнером.

	list<Point> l1(v1.begin(), v1.end());
	back_insert_iterator<vector<Point>> itbv = back_inserter(v1);
	*itbv = Point(9, 9);
	insert_iterator<set<Point>> its = inserter(s1, s1.begin());
	*its = Point(8, 8);
	front_insert_iterator<list<Point>> itfl = front_inserter(l1);
	*itfl = Point(5, 5);
	stop

	///////////////////////////////////////////////////////////////////

	//Задание 2. Обобщенные алгоритмы (заголовочный файл <algorithm>). Предикаты.

	// алгоритм for_each() - вызов заданной функции для каждого элемента любой последовательности
	//(массив, vector, list...)
	//С помощью алгоритма for_each в любой последовательности с элементами любого типа
	//распечатайте значения элементов
	//Подсказка : неплохо вызываемую функцию определить как шаблон

	for_each(l1.begin(), l1.end(), print_value<Point>);
	stop
	/*
	Value: 5 5
	Value: 2 3
	Value: 1 1
	Value: 0 0
	*/

	//С помощью алгоритма for_each в любой последовательности с элементами типа Point
	//измените "координаты" на указанное значение (такой предикат тоже стоит реализовать 
	//как шаблон) и выведите результат с помощью предыдущего предиката

	for_each(l1.begin(), l1.end(), plus_value(5));
	copy(l1.begin(), l1.end(), ostream_iterator<Point>(cout, " | "));
	cout << endl;
	stop
	//10 10 | 7 8 | 6 6 | 5 5 |

	//С помощью алгоритма find() найдите в любой последовательности элементов Point
	//все итераторы на элемент Point с указанным значением.

	vector<Point> v2;
	list<Point>::iterator itlb = l1.begin();
	while (itlb != l1.end())
	{
		itlb = find(itlb, l1.end(), Point(5, 5));
		if (itlb == l1.end()) // если ничего не нашли
		{
			break;
		}
		v2.push_back(*itlb); // заносим найденной значение
		++itlb;
	}
	copy(v2.begin(), v2.end(), ostream_iterator<Point>(cout, " | "));
	cout << endl;
	stop
	// 5 5 |

	//С помощью алгоритма sort() отсортируйте любую последовательность элементов Point. 
	////По умолчанию алгоритм сортирует последовательность по возрастанию.
	//Что должно быть определено в классе Point?
	// Замечание: обобщенный алгоритм sort не работает со списком, так как
	//это было бы не эффективно => для списка сортировка реализована методом класса!!!

	sort(v1.begin(), v1.end());
	copy(v1.begin(), v1.end(), ostream_iterator<Point>(cout, " | "));
	cout << endl;
	stop
	// 0 0 | 1 1 | 2 3 | 9 9 |

	//С помощью алгоритма find_if() найдите в любой последовательности элементов Point
	//итератор на элемент Point, удовлетворяющий условию: координаты x и y лежат в промежутке
	//[-n, +m].

	vector<Point> v3;
	itlb = l1.begin();
	while (itlb != l1.end())
	{
		itlb = find_if(itlb, l1.end(), point_range(-1,7));
		if (itlb == l1.end()) // если ничего не нашли
		{
			break;
		}
		v3.push_back(*itlb); // заносим найденной значение
		++itlb;
	}
	copy(v3.begin(), v3.end(), ostream_iterator<Point>(cout, " | "));
	cout << endl;
	stop
	//6 6 | 5 5 |

	//С помощью алгоритма sort() отсортируйте любую последовательность элементов Rect,
	//располагая прямоугольники по удалению центра от начала координат.

	vector<Rect> v4 = { Rect(2,3,1,2), Rect(1,2,3,4),  Rect(0,0,0,0), Rect(5,6,7,8) };
	copy(v4.begin(), v4.end(), ostream_iterator<Rect>(cout, " | "));
	cout << endl;
	// 2 3 2 1 | 1 2 4 3 | 0 0 0 0 | 5 6 8 7 |
	sort(v4.begin(), v4.end(), cmp_distance); // сортируем по собственному предикату
	copy(v4.begin(), v4.end(), ostream_iterator<Rect>(cout, " | "));
	cout << endl;
	// 0 0 0 0 | 2 3 2 1 | 1 2 4 3 | 5 6 8 7 |
	stop

	{//transform
		//Напишите функцию, которая с помощью алгоритма transform переводит 
		//содержимое объекта string в нижний регистр.
		//Подсказка: класс string - это "почти" контейнер, поэтому для него
		// определены методы begin() и end()

		string my_str("fiNiSH SOON");
		transform(my_str.begin(), my_str.end(), my_str.begin(), tolower);
		cout << my_str << endl;
		// finish soon
		stop

		//Заполните list объектами string. С помощью алгоритма transform сформируте
		//значения "пустого" set, конвертируя строки в нижний регистр

		list<string> l2 = { "Abba", "Alfa", "Beta", "Beauty", "Count", "City", "City", "Alfa" , "Beta", "Bounty" };
		set<string> s2;
		insert_iterator<set<string>> its = inserter(s2, s2.end());
		transform(l2.begin(), l2.end(), its, lower_string);
		copy(s2.begin(), s2.end(), ostream_iterator<string>(cout, " | "));
		cout << endl;
		stop
		// abba | alfa | beauty | beta | bounty | city | count |
	}
	return 0;
}

