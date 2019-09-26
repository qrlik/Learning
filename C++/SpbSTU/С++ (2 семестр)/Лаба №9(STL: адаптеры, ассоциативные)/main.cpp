// Контейнеры STL: 
//stack, queue, priority_queue
//set, multiset, map, multimap

// #include "stdafx.h"
#include <tchar.h>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <clocale>
#include <iostream>
#include "Header.h"
#include "point.h"

using namespace std;

#define stop __asm nop

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	//Напишите шаблон функции для вывода значений stack, queue, priority_queue
	//обратите внимание на то, что контейнеры предоставляют РАЗНЫЕ методы для 
	//получения значений

	////////////////////////////////////////////////////////////////////////////////////
	//stack

	//Создайте стек таким образом, чтобы
	//а) элементы стека стали копиями элементов вектора
	//б) при выводе значений как вектора, так и стека порядок значений был одинаковым 

	vector<int> v1 = { 1, 2, 3, 4 };
	stack<int, vector<int>> s1;
	vector<int>::reverse_iterator itrb = v1.rbegin(), itre = v1.rend(); // заносим элемнты реверсивно, чтобы печать совпала
	while (itrb != itre)
	{
		s1.push(*itrb);
		++itrb;
	}
	print_container(v1); // 1 | 2 | 3 | 4 |
	print_adapter(s1); // 1 | 2 | 3 | 4 |
	stop

		////////////////////////////////////////////////////////////////////////////////////
		//queue

		//Создайте очередь, которая содержит указатели на объекты типа Point,
		//при этом явно задайте базовый контейнер.
		//Измените значения первого и последнего элементов посредством front() и back()
		//Подумайте, что требуется сделать при уничтожении такой очереди?

		queue<Point*, deque<Point*>> q1;
	q1.push(new Point(1, 1));
	q1.push(nullptr);
	*q1.front() = Point(4, 1);
	q1.back() = new Point(2, 2);
	while (q1.size())
	{
		delete q1.front();
		q1.pop();
	}
	stop

		////////////////////////////////////////////////////////////////////////////////////
		//priority_queue
		//а) создайте очередь с приоритетами, которая будет хранить адреса строковых литералов - const char*
		//б) проинициализируйте очередь при создании с помощью вспомогательного массива с элементами const char*
		//в) проверьте "упорядоченность" значений (с помощью pop() ) - если они оказываются не упорядоченными, подумайте:
		//		что сравнивается при вставке?

		const char* arr[] = { "wwwfirst", "second", "third" };
	priority_queue<const char*, vector<const char*>, my_compare> pr1(arr, arr + (sizeof(arr) / sizeof(arr[0])));
	print_adapter(pr1); // wwwfirst | third | second |
	stop

		////////////////////////////////////////////////////////////////////////////////////
		//set
		//a) создайте множество с элементами типа Point - подумайте, что необходимо определить
		//		в классе Point (и каким образом) operator<
		//б) распечатайте значения элементов с помощью шаблона, реализованного в предыдущей лаб. работе
		//в) попробуйте изменить любое значение... // менять значения нельзя
		//г) Создайте два множества, которые будут содержать одинаковые значения
		//		типа int, но занесенные в разном порядке
		//д) Вставьте в любое множество диапазон элементов из любого другого
		//	контейнера, например, элементов массива	(что происходит, если в массиве имеются дубли?) // дубли игнорируются
		set<Point> set1;
	set1.insert(Point(1, 1));
	set1.insert(Point(2, 2));
	set1.insert(Point(0, 1));
	set1.insert(Point(1, 0));
	print_container(set1); // 0 1 | 1 1 | 2 2 |
	stop

		set<int> set2 = { 3, 4, 6, 7, 2, 10 };
	print_container(set2); // 2 | 3 | 4 | 6 | 7 | 10 |
	set<int> set3 = { 6, 3, 4, 2, 10, 7 };
	print_container(set3); // 2 | 3 | 4 | 6 | 7 | 10 |
	stop

		int arr2[] = { 1, 2, 3, 4, 5, 6 };
	set3.insert(arr2, arr2 + (sizeof(arr2) / sizeof(arr2[0])));
	print_container(set3); // 1 | 2 | 3 | 4 | 5 | 6 | 7 | 10 |
	stop

		////////////////////////////////////////////////////////////////////////////////////
		//map, multiset
		//а) создайте map, который хранит пары "фамилия, зарплата" - pair<const char*, int>,
		//	при этом строки задаются строковыми литералами

		map<const char*, int, my_compare> m1;

	//б) заполните контейнер значениями посредством operator[] и insert()

	m1["Popov"] = 20000;
	m1["Krainov"] = 10000;
	m1.insert(pair<const char*, int>("Ivanova", 30000));
	m1.insert(pair<const char*, int>("Maksimov", 5000));

	//в) распечатайте содержимое

	print_map(m1);
	stop
		/*
		Key: Ivanova | Value: 30000
		Key: Krainov | Value: 10000
		Key: Maksimov | Value: 5000
		Key: Popov | Value: 20000
		*/

		//г) замените один из КЛЮЧЕЙ на новый (была "Иванова", вышла замуж => стала "Петрова")

		pair<const char*, int> buf = *(m1.find("Ivanova")); // сохраняем данные Ивановой
	buf.first = "Petrova"; // меняем фамилию
	m1.erase("Ivanova"); // удаляем старый ключ
	m1.insert(buf); // заносим новый ключ со старыми данными
	print_map(m1);
	stop
		/*
		Key: Krainov | Value: 10000
		Key: Maksimov | Value: 5000
		Key: Petrova | Value: 30000
		Key: Popov | Value: 20000
		*/

		//д) Сформируйте любым способом вектор с элементами типа string.
		//Создайте (и распечатайте для проверки) map<string, int>, который будет
		//содержать упорядоченные по алфавиту строки и
		//количество повторений каждой строки в векторе

		vector<string> v2 = { "January", "February", "January", "March", "September", "September" };
	map<string, int> m2;
	vector<string>::const_iterator itcb = v2.cbegin(), itce = v2.cend();
	while (itcb != itce)
	{
		m2[*itcb]++; // добавляем и увеличиваем если нет, если есть просто увеличиваем счетчик. int() = 0
		++itcb; // итерируемся по вектору
	}
	print_map(m2);
	stop
		/*
		Key: February | Value: 1
		Key: January | Value: 2
		Key: March | Value: 1
		Key: September | Value: 2
		*/

		//е) 
		//задан массив:
		//const char* words[] = {"Abba", "Alfa", "Beta", "Beauty" ,...};
		//создайте map, в котором каждой букве будет соответствовать совокупность 
		//лексиграфически упорядоченных слов, начинающихся с этой буквы.
		//Подсказка: не стоит хранить дубли одной и той же строки
		//'A' -  "Abba" "Alfa"
		//'B' -  "Beauty" "Beta"  ...
		//...
		const char* words[] = { "Abba", "Alfa", "Beta", "Beauty", "Count", "City", "Alfa" , "Beta", "Bounty" };

	char ch = 'a'; //ключ 

	map<char, set<string>> m3;

	while (ch != '{') // для каждой буквы ascii
	{
		for (size_t i = 0; i < (sizeof(words) / sizeof(words[0])); i++) // заносим снова в словарь
		{
			if (tolower(words[i][0]) == ch)
			{
				m3[ch].insert(words[i]);
			}
		}
		ch++;
	}
	print_map(m3); // оператор перегружен под map<char,set<string>>
	stop
		/*
		Key: a | Value: Abba | Alfa |
		Key: b | Value: Beauty | Beta | Bounty |
		Key: c | Value: City | Count |
		*/

		//ж)
		//создайте структуру данных, которая будет хранить информацию о студенческих группах.
		//Для каждой группы должны хранится фамилии студентов (по алфавиту). При этом 
		//фамилии могут дублироваться
		//Сами группы тоже должны быть упорядочены по номеру
		//
		//номера 

		map<size_t, multiset<string>> m4;

	m4[1001] = { "Ivanova", "Popov", "Maksimov", "Ivanova" };
	m4[2001] = { "Kukuhin", "Markov", "Sidorov" };
	print_map(m4);
	stop
		/*
		Key: 1001 | Value: Ivanova | Ivanova | Maksimov | Popov |
		Key: 2001 | Value: Kukuhin | Markov | Sidorov |
		*/

		////////////////////////////////////////////////////////////////////////////////////
		//multimap
		//а) создайте "англо-русский" словарь, где одному и тому же ключу будут соответствовать
		//		несколько русских значений - pair<string,string>, например: strange: чужой, странный...

		multimap<wstring, pair<wstring, wstring>> mm1;

	//б) Заполните словарь парами с помощью метода insert или проинициализируйте с помощью 
	//		вспомогательного массива пара (пары можно конструировать или создавать с помощью шаблона make_pair)

	mm1.insert(pair<wstring, pair<wstring, wstring>>(L"fly", pair<wstring, wstring>(L"летать", L"муха")));
	mm1.insert(pair<wstring, pair<wstring, wstring>>(L"over", pair<wstring, wstring>(L"над", L"свыше")));
	mm1.insert(pair<wstring, pair<wstring, wstring>>(L"over", pair<wstring, wstring>(L"через", L"заканчиваться")));
	mm1.insert(pair<wstring, pair<wstring, wstring>>(L"accept", pair<wstring, wstring>(L"принимать", L"признавать")));
	mm1.insert(pair<wstring, pair<wstring, wstring>>(L"concern", pair<wstring, wstring>(L"касаться", L"заботиться")));

	//в) Выведите все содержимое словаря на экран

	print_multimap(mm1); // символы
	stop
	/*
	Key: accept | Value: принимать, признавать
	Key: concern | Value: касаться, заботиться
	Key: fly | Value: летать, муха
	Key: over | Value: над, свыше
	Key: over | Value: через, заканчиваться
	*/

	//г) Выведите на экран только варианты "переводов" для заданного ключа. Подсказка: для нахождения диапазона
	//		итераторов можно использовать методы lower_bound() и upper_bound()

	wstring str1 = L"over";
	print_mmkey(mm1, str1);
	stop
	// Key: over| Value: над свыше через заканчиваться

	return 0;
}

