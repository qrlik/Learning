//Практика 1:
// enum class
// initializer_list и универсальные списки инициализации
// auto
// decltype
// lambda functions

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <cassert>

#include "functions.h"

using namespace std;

//***********************************************
//enum и рефлексия
// https://habr.com/post/276763/ 
//************************************************

int main()
{
	//Задание 1. 
	{
		enum class months : unsigned char { January = 1, February, March/*...*/ };
		enum class weekDays : unsigned char { Monday = 1, Tuesday /*...*/ };

		months m = months::January;
		//а) уберите проблемы
		if (m == static_cast<months>(weekDays::Monday)) { /*...*/ }
		//б) оптимизируйте использование памяти
		months year[] = { months::January, months::February, months::March };
		size_t n = sizeof(year); // 3
		assert(n == 3);
	}

	//////////////////////////////////////////////////////////////////////////////////
	//Задание 2. Создайте и с помощью списка инициализации заполните значениями вектор с
	//элементами - string
	//С помощью range-based for преобразуйте все буквы в верхний регистр
	//а остальные символы должны остаться неизменными
	{
		vector<string> v = {"letters", "letters + 123", "@#$()#@123 what?"};
		for (auto& str : v) {
			transform(str.begin(), str.end(), str.begin(),
				[](unsigned char c) { return static_cast<char>(toupper(c)); });
		}
		vector<string> result = { "LETTERS", "LETTERS + 123", "@#$()#@123 WHAT?" };
		assert(v == result);
		__asm nop
	}

	/////////////////////////////////////////////////////////////////////
	///Задание 3. Создайте и с помощью списка инициализации заполните значениями
	//map таким образом, чтобы он содержал в качестве ключа букву, а в качестве значения 
	//отсортированную по алфавиту совокупность слов (string), начинающихся с этой буквы

	//С помощью range-based for распечатайте содержимое, например: A: any, apple, away
	{
		map<char, set<string>> m = { {'a', {"any", "apple", "away"}},
		{'o', {"orange", "oracle", "ohio", "ozzy"}},
		{'b', {"basket", "banana", "baloon"}}
		};

		ostringstream os;

		for (const auto& [letter, words] : m) {
			os << letter << ": ";
			bool first = true;
			for (const auto& word : words) {
				if (!first) {
					os << ", ";
				}
				else {
					first = false;
				}
				os << word;
			}
			os << '\n';
		}

		string result(
			"a: any, apple, away\n"
			"b: baloon, banana, basket\n"
			"o: ohio, oracle, orange, ozzy\n"
		);
		assert(os.str() == result);
		__asm nop
	}

	//3б. Создайте пустой map и используйте заданный массив
	//для выполнения задания.
	//С помощью range-based for и structured binding
	//распечатайте содержимое, например: A: any, apple, away
	{
		//дано (например):
		const char* s[] = { "yong", "away", "bar", "any", "son", "apple" };
		map<char, set<string>> m;
		for (auto ptr : s) {
			if (ptr && ptr[0] != '\0') {
				m[ptr[0]].emplace(string(ptr));
			}
		}

		ostringstream os;

		for (const auto& [letter, words] : m) {
			os << letter << ": ";
			bool first = true;
			for (const auto& word : words) {
				if (!first) {
					os << ", ";
				}
				else {
					first = false;
				}
				os << word;
			}
			os << '\n';
		}

		string result(
			"a: any, apple, away\n"
			"b: bar\n"
			"s: son\n"
			"y: yong\n"
		);
		assert(os.str() == result);

		__asm nop
	}

	/////////////////////////////////////////////////////////////////////
	///Задание 4. создать функцию для вывода на печать элементов последовательностей, заданных ниже
	{
		ostringstream os;

		vector<double> vd = { 1.1,2.2,3.3 };
		PrintAnyCont(vd, os);
		os << '\n';

		string s("abc");
		PrintAnyCont(s, os);
		os << '\n';

		int ar[] = { 1, 2, 3 };
		PrintAnyCont(ar, os);
		os << '\n';

		initializer_list<int> il{ 3,4,5 };
		PrintAnyCont(il, os);
		os << '\n';

		string result(
			"1.1 2.2 3.3 \n"
			"a b c \n"
			"1 2 3 \n"
			"3 4 5 \n"
		);
		assert(os.str() == result);
		__asm nop
	}

	/////////////////////////////////////////////////////////////////////
	///Задание 5. 	
	//создать функцию для "отрицания" значений, например:
	// было: {1, -2, 5}, стало: {-1, 2, -5})
	// изменение объектов типа std::string может выглядеть "aBc1" -> "AbC1"
	//элементов последовательностей, заданных ниже:
	{
		vector<double> vd{ 1.1,2.2,3.3 };
		NegateAll(vd);

		list<string> ls{ "aBc", "Qwerty", "n12" };
		NegateAll(ls);

		int ar[]{ 1, 2, 3 };
		NegateAll(ar);

		vector<double> vdN{ -1.1, -2.2, -3.3 };
		list<string> lsN{ "AbC", "qWERTY", "N12" };
		int arN[]{ -1, -2, -3 };

		assert(vd == vdN);
		assert(ls == lsN);
		assert(equal(cbegin(ar), cend(ar), cbegin(arN)));
		__asm nop

	}

	///Задание 5а. создать функцию для инкремента элементов последовательностей, заданных ниже
	{
		vector<double> vd = { 1.1, 2.2, 3.3 };
		IncAnyCont(vd);

		int ar[] = { 1, 2, 3 };
		IncAnyCont(ar);

		string s("123");
		IncAnyCont(s);

		vector<double> vdI{ 2.1, 3.2, 4.3 };
		int arI[]{ 2, 3, 4 };
		string sI("234");

		assert(vd == vdI);
		assert(equal(cbegin(ar), cend(ar), cbegin(arI)));
		assert(s == sI);
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////
	//Задание 6. Реализовать функцию сортировки по модулю элементов последовательностей, заданных ниже
	//Собственно для сортировки можно использовать обобщенный алгоритм sort(), 
	//а для задания условия - лямбда-функцию
	{
		vector<double> vd = { -3.3,  2.2, -1.1 };
		absSort(vd);

		int ar[] = { -3, 2, -1 };
		absSort(ar);

		vector vdS = { -1.1, 2.2, -3.3 };
		int arS[] = { -1, 2 , -3 };
		assert(vd == vdS);
		assert(equal(cbegin(ar), cend(ar), cbegin(arS)));
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////
	//Задание 7.
	//напишите функцию, которая будет формировать и возвращать вектор, каждый элемент 
	//которого является суммой элементов двух последовательностей 
	//РАЗНОЙ длины и с элементами РАЗНОГО типа.

	//Подсказка 1: так как последовательности могут быть разной длины, логично
	//сделать размер результирующего вектора максимальным из двух

	//Подсказка 2: подумайте о возможности использования алгоритма transform(), 
	//в котором трансформирующее действие удобно в свою очередь задавать лямбда-функцией

	//например:
	{
		vector<int> v{ 1,2,3,4 };
		list<double> l{ 1.1, 2.2, 3.3, 4.4, 5.5 };
		auto v_l = SumCont(v, l);


		list<int> ll{ 1, 2, 3, 4, 5, 6, 7, 8 };
		double ar[] = { 1.1, 2.2, 3.3, 4.4, 5.5 };
		auto ll_ar = SumCont(ar, ll);

		vector<double> v_lR = { 2.1, 4.2, 6.3, 8.4, 5.5 };
		vector<double> ll_arR = { 2.1, 4.2, 6.3, 8.4, 10.5, 6, 7, 8 };
		assert(v_l == v_lR);
		assert(ll_ar == ll_arR);
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////
	//Задание 8. Реализуйте функцию, которая принимает следующие параметры:	
	//сформированную последовательность любого типа с элементами любого типа, 
	//два (пустых) контейнера любого типа из vector, list, deque, set с элементами того же 
	//типа, что и у сформированной последовательности 

	//Функция должна "разложить" значения заданной последовательности в два пустых контейнера 
	//согласно заданному условию. Условие задать лябда-функцией
	//Исходная последовательность при этом не меняется
	{
		//Например:
		vector<int> v{ 1,2,3,4,5 };
		list<int> l; //сюда четные
		deque<int> d; //а сюда нечетные
		auto is_even = [](int value) {
			return value % 2 == 0;
		};
		Separate(v, l, d, is_even);

		list l_e{ 2, 4 };
		deque d_o{ 1, 3, 5 };
		assert(l == l_e);
		assert(d == d_o);
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////
	//Задание 9. C помощью алгоритма for_each()!!! (а не count_if()) посчитать сколько 
	//букв в верхнем регистре
	//  Использовать лямбда функцию
	{
		char s[] = "Hello World!";
		size_t count = 0;
		for_each(begin(s), end(s), [&count](unsigned char c) { if (isupper(c)) { ++count; } });
		assert(count == 2);
		__asm nop
	}

	/********************************************************/
	//Задание 11. Реализовать конвертацию enum в строковое представление  - enumToString
	// и наоборот - stringToEnum

	//Подсказки: 
	//***********
	//1. Соответствующие именованным константам строки все равно нужно где-то хранить =>
	//1.1 Именованные константы в перечислении должны быть уникальными => соответствующие строки
	//тоже должны быть уникальными, => уникальные строки могут быть использованы в качестве
	//ключа в std::map

	//1.2 а вот значения (соответствующие именованым константам)
	//могут быть любыми и даже могут повторяться (упрощаем задачу, считая, что значения НЕ повторяются) 
	//=> подходит контейнер std::map<string,<значение> >

	//1.3 Согласно стандарту С++11 переменные перечислимого типа могут быть разного размера и типа,
	//а хотелось бы обеспечить универсальную конвертацию 

	//***********
	//2. Так как типы перечислений разные, то enumToString и stringToEnum должны быть шаблонными
	//2.1 Пользователь может ошибиться или злонамеренно передать в функции значения, которым
	//   ничего не соответствует => защита!
	//2.2 В функциях enumToString и stringToEnum (в зависимости от типа перечисления) нужно будет
	//   анализировать соответствующий используемому типу перечисления контейнер

	//***********
	//3. Чтобы действия с map<string, <значение> > не зависили от типа перечисления, логично реализовать "обертку":
	/*
	template<typename T> struct EnumMap
	{
		static std::map<std::string, T> m_eMap;
		static const auto& getMap() { return m_eMap; }
	};
	*/

	{
		//Например:
		COLORS c1;
		try {
			c1 = stringToEnum<COLORS>("Red");
			auto Str = enumToString(c1);

			c1 = stringToEnum<COLORS>("blue");
		}
		catch (const exception& ex)
		{
			cerr << ex.what() << endl;
		}

		__asm nop
	}
	return 0;
}
