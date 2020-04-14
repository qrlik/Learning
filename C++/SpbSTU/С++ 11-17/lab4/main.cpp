#include <iostream>
#include <vector>
#include <deque>
#include <set>
#include <algorithm>
#include <string>
#include <cassert>
#include <stack>

#include "functions.h"

using namespace std;

int main()
{
	/***************************************************************/
	//Задание 1.
		/*
		Реализуйте шаблон функции для печати любых последовательностей (vector, list, deque, set и встроенного массива), которые могут содержать:
		•	как объекты любого типа,
		•	так и указатели на объекты любого типа (указатели распечатывать неинтересно => в этом случае следует получать значение по адресу)
		Подсказки: if constexpr
		*/
	{
		char test_ar[5] = { 'a', 'b', 'c', 'd', 0 };
		PrintAnyCont(test_ar);
		cout << '\n';

		int i1 = 1;
		int i2 = 2;
		int i3 = 3;
		deque<int*> test_deque{ &i1, &i2, &i3 };
		PrintAnyCont(test_deque);
		cout << '\n';

		vector<const char*> test_vector{"first", "second", "third"};
		PrintAnyCont(test_vector);
		cout << "\n\n";
	}

	/***************************************************************/
	//Задание 2.
		/* Реализуйте шаблон функции сложения двух значений.
		Если первое слагаемое является вектором, то все элементы вектора нужно увеличить на значение второго параметра.
		При этом элементы вектора и второй параметр должны быть одного и того же типа.
		Подсказки: if constexpr, is_same
		*/
	{
		int i1 = 1;
		Summation(i1, 2);
		assert(i1 == 3);

		vector<int> test_v{ 1, 2, 3, 4 };
		Summation(test_v, -1);
		vector<int> result{ 0, 1, 2, 3 };
		assert(test_v == result);
	}


	/***************************************************************/
	//Задание 3.
	/*Реализуйте шаблон функции вывода на печать значений элементов любого адаптера (stack, queue, priority_queue)
	Подсказки: if constexpr, is_same
	Предусмотрите вывод значений, если в адаптере хранятся указатели.
	*/
	{
		deque<int> source{ 1, 3, 4, 2, 6 };
		queue<int> test_queue(source);
		PrintAnyAdap(test_queue); // 1 3 4 2 6
		cout << '\n';

		stack<int> test_stack(source);
		PrintAnyAdap(test_stack); // 6 2 4 3 1
		cout << '\n';

		priority_queue<int> test_prqueue;
		for (auto i : source) {
			test_prqueue.push(i); // 6 4 3 2 1
		}
		PrintAnyAdap(test_prqueue);
		cout << "\n\n";
	}

	/***************************************************************/
	//Задание 4.
	{
		//Дан массив элементов типа string
		string strings[] = { "abc", "123", "526", "qwerty", "#$%" };
		//До завершения фрагмента строки должны существовать в единственном экземпляре.
		//Требуется обеспечить манипулирование строками а) без копирования и б) без изменения порядка
		//элементов в массиве!
		auto my_deleter = [](const string*) {};

		//В std::set "складываем" по алфавиту обертки для строк, которые содержат только буквы 
		set<shared_ptr<const string>, SharedCompare> set_str;
		for (const auto& str : strings) {
			if (all_of(str.begin(), str.end(),
				[](unsigned char c) { return isalpha(c); })) {
				set_str.emplace(&str, my_deleter);
			}
		}

		for (auto ptr : set_str) {
			if (ptr) {
				cout << *ptr << ' ';
			}
		}
		cout << '\n';

		__asm nop
		/******************************************************************************************/

		//В std::vector "складываем" обертки для строк, которые содержат только цифры 
		//Выводим на экран
		//Находим сумму

		vector<shared_ptr<const string>> vector_ints;
		for (const auto& str : strings) {
			if (all_of(str.begin(), str.end(),
				[](unsigned char c) { return isdigit(c); })) {
				vector_ints.emplace_back(&str, my_deleter);
			}
		}

		int summ = 0;
		for (auto ptr : vector_ints) {
			if (ptr) {
				cout << *ptr << ' ';
				summ += stoi(*ptr);
			}
		}
		cout << '\n' << summ << '\n';

		/******************************************************************************************/
		//сюда "складываем" обертки для строк, которые не содержат ни символов букв, ни символов цифр
		//и просто выводим

		vector<shared_ptr<const string>> vector_symbs;
		for (const auto& str : strings) {
			if (all_of(str.begin(), str.end(),
				[](unsigned char c) { return !isalnum(c); })) {
				vector_symbs.emplace_back(&str, my_deleter);
			}
		}

		for (auto ptr : vector_symbs) {
			if (ptr) {
				cout << *ptr << ' ';
			}
		}
		cout << "\n\n";
	}


	/******************************************************************************************/
	//Задание 5. 
	{
		//Дано:
		const string ar[] = { "my", "Hello", "World" };
		vector<shared_ptr<const string>> v = { make_shared<string>("good"), make_shared<string>("bye") };

		transform(begin(ar), end(ar), back_inserter(v), [](const string& str)
			{ return shared_ptr<const string>(&str, [](const string*) {}); } );

		//а) Требуется добавить в вектор обертки для элементов массива, НЕ копируя элементы массива! 
		//б) Отсортировать вектор по алфавиту и вывести на экран
		//в) Обеспечить корректное освобождение памяти

		sort(v.begin(), v.end(), [](shared_ptr<const string> lhs, shared_ptr<const string> rhs)
			{ return *lhs < *rhs; });
		for (auto ptr : v) {
			if (ptr) {
				cout << *ptr << ' ';
			}
		}
		cout << "\n\n";

		__asm nop
	}

	/***************************************************************/
		//Задание 6. shared_ptr и weak_ptr
		//Создаем генеалогическое дерево посредством класса human. В классе хранятся:
		//имя - string
		//возможно признак: жив или уже нет...
		//родители - shared_ptr (родители не всегда известны...)
		//дети - контейнер из weak_ptr (чтобы избежать циклических зависимостей)

		//Методы класса human:
		//конструктор - для инициализации имени и признака
		//конструктор копирования, оператор присваивания, move ???
		//статический метод child() - 
		//				должен создать создать и вернуть обертку для родившегося человека
		//				+ сформировать все связи ребенка с родителями и наоборот

		//Ввести возможность распечатать генеалогическое дерево для указанного индивидума

	{
		//История должна с кого-то начинаться => "Жили-были дед да баба, например, Адам и Ева"
		//(то есть на самом деле два деда и две бабы):


		auto gragrandMother = make_shared<human>("Eva", false);
		auto gragrandFather = make_shared<human>("Adam", false);

		//у них появились дети - child():
		auto grandMother = human::child(human{ "Kate", false }, gragrandMother, gragrandFather);

		//а у детей в свою очередь свои дети:
		auto Father = human::child(human{ "Genry", false }, grandMother);
		auto Mother = human::child(human{ "Lilith", true });


		auto Son = human::child(human{ "Sasha", true }, Father, Mother);

		Son->print();
		//Eva
		//		Kate
		//Adam		
		//			Genry
		//					Sasha
		//			Lilith
		__asm nop
	}

	//***************************************************************/
	//Задание 7.

	/*Пользовательский deduction guide – для вывода типов параметров шаблона
	Задан шаблон класса, который инкапсулирует внедренный ограниченный массив известной
	размерности с элементами любого типа. */

	//Требуется обеспечить работоспособность приведенных примеров использования.
	{
		MyArray<int, 5> ar1;//MyArray<int,5>
		MyArray ar2{ "ABC" }; //MyArray<char,4>
		int ar[] = { 1,2,3 };
		MyArray ar3{ ar };

		__asm nop
	}
	return 0;
}
