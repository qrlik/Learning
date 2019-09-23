//Стандартная библиотека шаблонов - STL 
//Контейнеры стандартной библиотеки - vector
//Итераторы

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include "Header.h"
#include "MyString.h"
#include "point.h"

#pragma warning(disable: 4786)

using namespace std;

#define	  stop __asm nop


int main()
{
	// Контейнер стандартной библиотеки vector 

//////////////////////////////////////////////////////////////////////
	//Создание векторов. 

	//Создайте следующие вектора:
	//---------------------------
	//пустой вектор целых чисел - vInt и проверьте с помощью функции size(),
	//что его размер нулевой. 

	vector<int> vInt;
	cout << vInt.size() << endl; // 0


	//Раскомментируйте следующий фрагмент, подумайте - все ли корректно
	//Если есть некорректности, - исправьте

	vInt.push_back(1);
	stop

	//С помощью функции push_back() в цикле заполните вектор какими-либо значениями.
	//На каждой итерации цикла следите за размером вектора с помощью методов
	//size(), capacity(), max_size() и выводите значения элементов. 
	//
	//Подсказка: так как такая проверка понадобится Вам и в следующих заданиях,
	//напишите шаблон функции, которая для вектора, содержащего элементы любого типа
	//выводит его "реквизиты" и значения элементов на консоль.

	for (int i = 0; i < 5; i++)
	{
		vInt.push_back(i); // size - capacity;
		print_info<int>(vInt); // 2 - 2; 3 - 3; 4 - 4; 5 - 6; 6 - 6
	}
	stop

	//вектор вещественных - vDouble1 с заданным Вами начальным размером  и
	//проверьте результат с помощью созданного Вами шаблона. Как будут
	//проинициализированы элементы вектора?

	vector<double> vDouble1(10);
	print_info<double>(vDouble1); // 10 нулей
	stop

	//вектор объектов типа MyString с начальным размером - 5 элементов
	//и инициализацией каждого элемента строкой "A"
	//C помощью функции at() а также с помощью оператора
	//индексирования []  измените значения каких-либо элементов.
	//Попробуйте "выйти" за границы вектора с помощью at() и
	//с помощью []. 

	vector<MyString> vMyStr(5, MyString("A"));
	vMyStr[1] = "B";
	vMyStr.at(2) = "C";
	print_info<MyString>(vMyStr);
	//cout << vMyStr[5] << endl;
	try
	{
		cout << vMyStr.at(5) << endl;
	}
	catch (exception& e)
	{
		cout << e.what() << endl; // invalid vector<T> subscript
	}
	stop

	//вектор вещественных - vDouble3, который является копией элементов
	// [0,5) массива вещественных чисел dMas. Предворительно массив dMas 
	//нужно создать и проинициализировать!

	vector<double> dMas;
	for (int i = 0; i < 10; i++)
	{
		dMas.push_back(i);
	}
	vector<double> vDouble3;
	vDouble3.insert(vDouble3.begin(), dMas.begin(), dMas.begin() + 5);
	print_info<double>(dMas);
	print_info<double>(vDouble3);
	stop
	/*
	Size:   10 Capacity:    13 Max_size:    536870911
	0 1 2 3 4 5 6 7 8 9
	Size:   5 Capacity:     5 Max_size:     536870911
	0 1 2 3 4
	*/

	//вектор вещественных - vDouble4, который является копией элементов
	// [2,5) вектора vDouble3. 

	vector<double> vDouble4(vDouble3.begin() + 2, vDouble3.begin() + 5);
	print_info<double>(vDouble4);
	stop
	/*
	Size:   3 Capacity:     3 Max_size:     536870911
	2 3 4
	*/

	//вектор элементов типа Point - vPoint1 а) с начальным размером 3. Какой конструктор
	//будет вызван для каждого элемента?
	//b) vPoint2 с начальным размером 5 и проинициализируйте каждый элемент координатами (1,1).

	vector<Point> vPoint1(3); // default
	vector<Point> vPoint2(5, Point(1,1));

	//вектор указателей на Point - vpPoint с начальным размером 5
	vector<Point*> vpPoint(5);
	//Подумайте: как корректно заставить эти указатели "указывать" на объекты Point
	{
		for (int i = 0; i < 5; i++)
		{
			vpPoint[i] = new Point(i, i + 1);
		}
		//Подсказка: для вывода на печать значений скорее всего Вам понадобится
			//а) специализация Вашей шаблонной функции
			//б) или перегрузка operator<< для Point*
		print_info(vpPoint);
		for (int i = 0; i < 5; i++)
		{
			delete vpPoint[i];
		}
	}//Какие дополнительные действия нужно предпринять для такого вектора?
	stop

	///////////////////////////////////////////////////////////////////////
	//Резервирование памяти.
	//Подумайте, всегда ли верны приведенные ниже проверки?
	/*
	{
	size_t n=...
	vector<int> v(n);
	v.resize(n/2);
	if(v.capacity() == n) //true, уменьшается size, capacity остается старым
	}
	*/

	/*
	{
	int n=...
	size_t m=...
	vector<int> v(n);
	v.reserve(m);
	if(v.capacity() == m) //true if m > n )
	}
	*/

	/*
	{
	vector<int> v(3,5); 5 5 5
	v.resize(4,10); //значения? 10 10 10 10
	v.resize(5); //значения? 10 10 10 10 0
	}
	*/

	//Создайте два "пустых" вектора с элементами
	//любого (но одного и того же типа) типа. 
	//В первом векторе зарезервируйте память под 5 элементов, а потом заполните
	//его значениями с помощью push_back.
	//Второй вектор просто заполните значениями посредством push_back.
	//
	//Сравните размер, емкость векторов и значения элементов

	vector<int> vInt1;
	vInt1.reserve(5);
	vector<int> vInt2;
	for (int i = 0; i < 5; i++)
	{
		vInt1.push_back(i);
		vInt2.push_back(i);
	}
	print_info<int>(vInt1);
	print_info<int>(vInt2);
	//Size:   5 Capacity : 5 Max_size : 1073741823
	//0 1 2 3 4
	//Size : 5 Capacity : 6 Max_size : 1073741823
	//0 1 2 3 4
	stop

	//!!! shrink_to_fit - Уменьшение емкости вектора.
	//Для любого вектора из предыдущего задания требуется уменьшить емкость
	//до size.

	vInt2.shrink_to_fit(); // уменьшаем емкость до size
	print_info<int>(vInt2);
	//Size : 5 Capacity : 5 Max_size : 1073741823
	//0 1 2 3 4
	stop

	//Создание "двухмерного вектора" - вектора векторов
	//Задан одномерный массив int ar[] = {11,2,4,3,5};
	//Создайте вектор векторов следующим образом:
	//вектор vv[0] - содержит 11 элементов со значением 11
	//vv[1] - содержит 2,2
	//vv[2] - содержит 4,4,4,4
	//...
	//Распечатайте содержимое такого двухмерного вектора по строкам

	int ar[] = { 11,2,4,3,5 };
	vector<vector<int>> vv;
	vv.reserve(sizeof(ar) / sizeof(ar[0]));
	for (int i = 0; i < sizeof(ar) / sizeof(ar[0]); i++)
	{
		vv.emplace_back(ar[i], ar[i]); // вставляем vector<int>(количество, значение)
	}
	for (int i = 0; i < vv.size(); i++)
	{
		print_info<int>(vv[i]);
	}
	std::cout << vv;
	stop
	/*
	Size:   11 Capacity:    11 Max_size:    1073741823
	11 11 11 11 11 11 11 11 11 11 11
	Size:   2 Capacity:     2 Max_size:     1073741823
	2 2
	Size:   4 Capacity:     4 Max_size:     1073741823
	4 4 4 4
	Size:   3 Capacity:     3 Max_size:     1073741823
	3 3 3
	Size:   5 Capacity:     5 Max_size:     1073741823
	5 5 5 5 5

	11 11 11 11 11 11 11 11 11 11 11
	 2 2
	 4 4 4 4
	 3 3 3
	 5 5 5 5 5
	*/

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Вставка элемента последовательности insert().
	//В вектор vChar2 вставьте в начало вектора символ только при
	//условии, что в векторе такого еще нет.

	vector<char> vChar2(5, 'a');
	insertChar(vChar2, 'b');
	print_info<char>(vChar2);
	stop
	/*
	Size:   6 Capacity:     7 Max_size:     2147483647
	b a a a a a
	*/

	//Вставьте перед каждым элементом вектора vChar2 букву 'W'
	vector<char>::iterator it = vChar2.begin();
	size_t N = vChar2.size();
	for (int i = 0; i < N; i++)
	{
		it = vChar2.insert(it, 'W'); // it = чтобы итератор не уехал
		it += 2;
	}
	print_info(vChar2);
	stop
	/*
	Size:   12 Capacity:    15 Max_size:    2147483647
	W b W a W a W a W a W a
	*/

	///////////////////////////////////////////////////////////////////
	//Напишите функцию, которая должна удалять только повторяющиеся последовательности.
	//Например: было - "qwerrrrty12222r3", стало - "qwety1r3"

	char my_str[] = "qwerrrrty12222r3";
	stop
	vector<char> str1(my_str, my_str + strlen(my_str));
	noPair(str1);
	print_info(str1);
	stop
	/*
	Size:   8 Capacity:     16 Max_size:    2147483647
	q w e t y 1 r 3
	*/

	///////////////////////////////////////////////////////////////////
	//Удаление элемента последовательности erase()
	//Напишите функцию удаления из любого вектора всех дублей 

	vector<char> str2(my_str, my_str + strlen(my_str));
	noDublies(str2);
	print_info(str2);
	stop
	/*
	Size:   9 Capacity:     16 Max_size:    2147483647
	q w e r t y 1 2 3
	*/

	///////////////////////////////////////////////////////////////////
	//Создайте новый вектор таким образом, чтобы его элементы стали
	//копиями элементов любого из созданных ранее векторов, но расположены
	//были бы в обратном порядке

	vector<int> v1 = { 1, 2, 3, 4, 5 };
	vector<int> v2(v1.rbegin(), v1.rend());
	print_info(v2);
	stop
	/*
	Size:   5 Capacity:     5 Max_size:     1073741823
	5 4 3 2 1
	*/

	///////////////////////////////////////////////////////////////////
	//Задание 1. Списки. Операции, характерные для списков.
	//Создайте пустой список из элементов Point - ptList1 и наполните
	//его значениями с помощью методов push_back(),
	//push_front, insert()
	list<Point> lp1;
	lp1.push_front(Point(1, 1));
	lp1.push_back(Point(3, 3));
	lp1.insert(lp1.begin(), Point(2, 2));

	//Напишите шаблон функции, которая будет выводить элементы
	//ЛЮБОГО КОНТЕЙНЕРА на печать. Проверьте работу шаблона на контейнерах
	//vector и list. Подсказка - хотелось бы увидеть тип контейнера.

	print_container(lp1);
	stop
	/*
	class std::list<class Point,class std::allocator<class Point> >
	2 2 | 1 1 | 3 3 |
	*/

	//Сделайте любой из списков "реверсивным" - reverse()
	lp1.reverse();
	print_container(lp1);
	stop
	/*
	class std::list<class Point,class std::allocator<class Point> >
	3 3 | 1 1 | 2 2 |
	*/

	//Создайте список ptList2 из элементов Point таким образом, чтобы он стал 
	//копией вектора элементов типа Point, но значения элементов списка располагались
	//бы в обратном порядке 
	vector<Point> vp1(lp1.begin(), lp1.end());
	list<Point> ptList2(lp1.rbegin(), lp1.rend());
	print_container(ptList2); 
	/*
	class std::list<class Point,class std::allocator<class Point> >
	2 2 | 1 1 | 3 3 |
	*/
	stop

	//Отсортируйте списки  ptList1 и ptList2 - методом класса list - sort()
	//по возрастанию.
	//Подумайте: что должно быть перегружено в классе Point для того, чтобы
	//работала сортировка

	lp1.sort(); // operator<
	ptList2.sort();
	print_container(lp1);
	print_container(ptList2);
	stop
	/*
	class std::list<class Point,class std::allocator<class Point> >
	1 1 | 2 2 | 3 3 |
	*/

	//Объедините отсортированные списки - merge(). Посмотрите: что
	//при этом происходит с каждым списком.
	lp1.merge(ptList2);
	print_container(lp1);
	print_container(ptList2); // empty
	stop
	/*
	class std::list<class Point,class std::allocator<class Point> >
	1 1 | 1 1 | 2 2 | 2 2 | 3 3 | 3 3 |
	*/

	//Исключение элемента из списка - remove()
	//Исключите из списка элемент с определенным значением.
	//Подумайте: что должно быть перегружено в классе Point?

	lp1.remove(Point(1, 1)); //operator==
	print_container(lp1);
	stop
	/*
	class std::list<class Point,class std::allocator<class Point> >
	2 2 | 2 2 | 3 3 | 3 3 |
	*/

	//Исключение элемента из списка, удовлетворяющего заданному условию:
	//любая из координат отрицательна - remove_if(). 

	lp1.push_front(Point(-1, 0));
	print_container(lp1);
	lp1.remove_if(IfNeg);
	print_container(lp1);
	stop

	//Исключение из списка подряд расположенных дублей - unique(). 

	lp1.unique();
	print_container(lp1);
	stop
	/*
	class std::list<class Point,class std::allocator<class Point> >
	2 2 | 3 3 |
	*/

	///////////////////////////////////////////////////////////////////
	//Задание 2.Очередь с двумя концами - контейнер deque
	//Создайте пустой deque с элементами типа Point. С помощью
	//assign заполните deque копиями элементов вектора. С помощью
	//разработанного Вами в предыдущем задании универсального шаблона
	//выведите значения элементов на печать

	deque<Point> my_deq;
	my_deq.assign(vPoint2.begin(), vPoint2.end());
	//my_deq.insert(my_deq.begin(), vPoint2.begin(), vPoint2.end());
	print_container(my_deq);
	stop
	/*
	class std::deque<class Point,class std::allocator<class Point> >
	1 1 | 1 1 | 1 1 | 1 1 | 1 1 |
	*/

	//Создайте deque с элементами типа MyString. Заполните его значениями
	//с помощью push_back(), push_front(), insert()
	//С помощью erase удалите из deque все элементы, в которых строчки
	//начинаются с 'A' или 'a'

	deque<MyString> my_deq2;
	my_deq2.push_back("a");
	my_deq2.push_back("b");
	my_deq2.push_front("A");
	my_deq2.push_front("B");
	my_deq2.insert(my_deq2.begin(), "C");
	print_container(my_deq2);
	stop
	/*
	class std::deque<class MyString,class std::allocator<class MyString> >
	Name: C | Name: B | Name: A | Name: a | Name: b |
	*/

	deque<MyString>::iterator it_deq = my_deq2.begin();
	while (it_deq != my_deq2.end())
	{
		if (it_deq->getString()[0] == 'a' || it_deq->getString()[0] == 'A')
		{
			it_deq = my_deq2.erase(it_deq);
		}
		else
		{
			it_deq++;
		}
	}
	print_container(my_deq2);
	stop
	//class std::deque<class MyString, class std::allocator<class MyString> >
	//Name : C | Name : B | Name : b |

	return 0;
}
