#include <iostream>
#include <cassert>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <memory>
#include <fstream>
#include <random>

using namespace std;

constexpr size_t Factorial(size_t value) {
	return (value) ? value * Factorial(value - 1) : 1;
}

constexpr size_t BinaryToDecimal(const char* c, size_t value) {
	if (*c == '0' || *c == '1') {
		return BinaryToDecimal(c + 1, value * 2 + (*c - '0'));
	}
	else {
		return value;
	}

	//if (*c == '1') {
	//	return BinaryToDecimal(++c, value * 2 + 1);
	//}
	//else if (*c == '0') {
	//	return BinaryToDecimal(++c, value * 2);
	//}
	//else {
	//	return value;
	//}
}

constexpr size_t operator"" _b(const char* c) {
	while (*c != '1' && *c != 0) { // пропуск мусора
		++c;
	}
	return (*c) ? BinaryToDecimal(++c, 1) : 0;
}

string operator"" _toBinStr(unsigned long long v) {
	if (v == 0) {
		return "0b0";
	}
	else {
		string result("0b");
		string bits;
		while (v > 0) {
			bits.push_back((v & 1) ? '1' : '0');
			v >>= 1;
		}
		result.insert(result.end(), bits.rbegin(), bits.rend());
		return result;
	}
}

template<typename T>
class BoundControl {
	const T _min{};
	const T _max{};
public:
	constexpr BoundControl() = default;
	constexpr BoundControl(const T& f, const T& s) :
		_min(min(f, s)), _max(max(f, s)) {}

	constexpr const T& getMin() const {
		return _min;
	}
	constexpr const T& getMax() const {
		return _max;
	}
	constexpr bool is_include(const T& value) const {
		return value >= _min && value <= _max;
	}
	constexpr const T& validate(const T& value) const {
		if (is_include(value)) {
			return value;
		}
		else if (value < _min) {
			return _min;
		}
		else {
			return _max;
		}
	}
};

int main()
{
	//Задание 1. Сырые строковые литералы (Raw String Literals)
	//Выведите строку, например: my name is "Marina"
	//а) без использования Raw String Literals
	//б) посредством Raw String Literals
	//в) добавьте разделители (Delimeter)

	{
		cout << "my name is \"Marina\"\n";
		cout << R"(my name is "Marina")" << '\n';
		cout << R"aaa(my name is "Marina")aaa" << '\n';
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//Задание 2. Реализуйте вычисление факториала с помощью constexpr-функции.
	//
	//Подсказки/напоминания: 
	//		- constexpr – функция должна состоять из единственной инструкции return <выражение>; (пока!)
	//		- но это выражение может включать вызов другой constexpr – функции, в частности рекурсивный
	//		  вызов 
	//		- если параметр рекурсивной constexpr- функции - это константа, компилятор вычислит результат
	//		  на этапе компиляции

	//Проверьте тот факт, что компилятор вычисляет значение на этапе компиляции (если в качестве
	//				параметра используется константа, известная компилятору на этапе компиляции).
	//				Для проверки достаточно создать встроенный массив с размерностью, вычисляемой
	//				посредством constexpr-функции:

	{	//Например:
		//int ar[Factorial(3)];

		//или
		//constexpr int n = Factorial(5);
		//int ar1[n];

		//попробуйте:
		//int m = 7;
		//constexpr int n1 = Factorial(m); // m - not const
		//int ar1[n1];

		//а так?
		//int n2 = Factorial(m); // run-time
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//Задание 3a. Перевод с помощью пользовательского литерала из двоичного представления строкового
	//в значение, например: строку "100000000" -> в значение 256
	//Проверить результат посредством префикса 0b
	//Чтобы вызов пользовательского литерала выглядел просто и читаемо, например: 100000000_b
	//логично использовать пользовательский литерал с единственным параметром - const char*

	//Так как речь идет о литералах, логично вычислять значения на этапе компиляции
	// => реализуйте пользовательский литерал посредством constexpr - функций
	//Подсказка/напоминание: 
	//		- constexpr – функция должна состоять из единственной инструкции return <выражение>;
	//		- но это выражение может включать вызов другой constexpr – функции,
	//		- которая может быть рекурсивной (если параметр такой функции - это константа,
	//		  компилятор вычислит результат вызова рекурсивной функции на этапе компиляции)

	{
		{
			constexpr size_t r1 = 0_b;
			static_assert(r1 == 0b0);
		}
		{
			const size_t r2 = 1_b;
			static_assert(r2 == 0b1);
		}
		{
			constexpr size_t r3 = 100000000_b;
			static_assert(r3 == 0b100000000);
		}
		{
			const size_t r4 = 1110010110001010_b;
			static_assert(r4 == 0b1110010110001010);
		}
		{
			const size_t r_max = 11111111111111111111111111111111_b;
			static_assert(r_max == 0b11111111111111111111111111111111);
		}
		__asm nop

	}

	//Задание 3b. Перевод в строковое двоичное представление, например: 256 -> "0b100000000"
	//Так как строка может быть любой длины, логичнее и проще возвращать объект std::string
	//=> возвращаемое значение не может быть constexpr!
	//Подсказка: манипулятора std::bin пока нет => преобразование в двоичную строку
	//придется делать вручную
	//Подсказка: количество разрядов в байте определяет константа CHAR_BIT - <cstdint>

	{
		{
			string sBin = 0_toBinStr;
			assert(sBin == "0b0");
		}
		{
			string sBin = 1_toBinStr;
			assert(sBin == "0b1");
		}
		{
			string sBin = 256_toBinStr;
			assert(sBin == "0b100000000");
		}
		{
			string sBin = 58762_toBinStr;
			assert(sBin == "0b1110010110001010");
		}
		{
			string sBin = 4294967295_toBinStr;
			assert(sBin == "0b11111111111111111111111111111111");
		}
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//Задание 4а. constexpr - объекты 
	//Создать класс (шаблон класса?) для хранения и манипулирования диапазоном значений.
	//В классе должны быть:
	//	переменные для хранения минимального и максимального значений,
	//	методы для получения каждого из значений
	//	метод для проверки - попадает ли указанное значение в диапазон
	//	метод, который получает любое значение данного типа и формирует результирующее значение:
	//							если принадлежит диапазону, то его и возвращаем
	//							если меньше минимального значения, возвращаем минимальное
	//							если больше максимального значения, возвращаем максимальное

	//Проверьте тот факт, что компилятор вычисляет значение на этапе компиляции. 
	//				Для проверки достаточно создать встроенный массив с размерностью, вычисляемой
	//				посредством constexpr-метода:
	{
		constexpr BoundControl c(3, 10);
		constexpr int ch1 = c.getMin();
		constexpr int ch2 = c.getMax();
		cout << boolalpha << c.is_include(5) << '\n'; // проверка на стадии компиляции, вывод run-time
		constexpr int ch3 = c.validate(11);
		static_assert(ch3 == 10);
		__asm nop
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//Задание 5. unique_ptr
	{

		//5.а - обеспечьте корректное выполнение фрагмента
		{
			vector<string*> v = { new string("aa"), new string("bb"), new string("cc") };
			//Распечатайте все строки

			cout << "\n === 5a === \n";
			for (auto ptr : v) { // nullptr check
				cout << *ptr << '\n';
			}
			cout << '\n';

			for (auto ptr : v) {
				delete ptr;
			}
			__asm nop
		}

		//5.b - модифицируйте задание 5.а:
		 //обеспечьте посредством std::unique_ptr: 
		 //эффективное заполнение (вспомните про разные способы формирования std::unique_ptr), 
		 //безопасное хранение указателей на динамически создаваемые объекты std::string, 
		 //манипулирование,
		 //и освобождение ресурсов
		 //

		{
			using MyString = unique_ptr<string>;
			vector<MyString> v;
			v.emplace_back(make_unique<string>("aa"));
			v.emplace_back(make_unique<string>("bb"));
			v.emplace_back(make_unique<string>("cc"));

			//Распечатайте все строки
			cout << "\n === 5b === \n";
			for (const auto& ptr : v) { // check for empty ptr
				cout << *ptr << '\n';
			}
			cout << '\n';
			__asm nop
			//??? Уничтожение динамически созданных объектов?
		 //???
		//5.c - дополните задание 5.b добавьте возможность изменять хранящиеся строки
		 //следующим образом (например, добавить указанный суффикс: "AAA" -> "AAA_1")  
			for (auto& ptr : v) {
				ptr->operator+=("_1");
			}

			__asm nop
		}

		{//5.d - динамический массив объектов	

		 //Создайте unique_ptr, который является оберткой для динамического массива
		 //с элементами std::string
		 //С помощью unique_ptr::operator[] заполните обернутый массив значениями
		 //Когда происходит освобождения памяти?
			const size_t n = 5;
			auto unique_strs = make_unique<string[]>(n);
			for (size_t i = 0; i < n; ++i) {
				unique_strs[i].push_back(static_cast<char>(i + 'a'));
			}

			cout << "\n === 5d === \n";
			for (size_t i = 0; i < n; ++i) {
				cout << unique_strs[i] << '\n';
			}
			cout << '\n';

			__asm nop
		}

		{//5.e - массивы динамических объектов и пользовательская delete-функция (функтор)
		 //Задан стековый массив указателей на динамически созданные объекты
		 //Создайте unique_ptr для такого массива
		 //Реализуйте пользовательскую delete-функцию (функтор) для корректного 
		 //освобождения памяти

			string* arStrPtr[] = { new string("aa"), new string("bb"), new string("cc") };

			constexpr size_t arr_size = size(arStrPtr);
			auto Deleter = [arr_size](string** ptr) {
				for (size_t i = 0; i < arr_size; ++i) {
					delete ptr[i];
				}
			};

			unique_ptr<string* [], decltype(Deleter)> unique_ptrs(new string*[arr_size], Deleter);
			for (size_t i = 0; i < arr_size; ++i) {
				unique_ptrs[i] = arStrPtr[i];
				arStrPtr[i] = nullptr;
			}

			__asm nop
		}

		{//5.f Создайте и заполните вектор, содержащий unique_ptr для указателей на std::string
			//Посредством алгоритмя copy() скопируйте элементы вектора в пустой список с элементами 
			//того же типа
			//Подсказка: перемещающие итераторы и шаблон std::make_move_iterator
			using MyString = unique_ptr<string>;
			vector<MyString> v;
			v.emplace_back(make_unique<string>("aaa"));
			v.emplace_back(make_unique<string>("cas"));
			v.emplace_back(make_unique<string>("bas"));
			v.emplace_back(make_unique<string>("das"));

			list<MyString> l(make_move_iterator(v.begin()),
				make_move_iterator(v.end()));

			__asm nop

		}
		__asm nop

	}

	////////////////////////////////////////////////////////////////////////////////
	//Задание 6.shared_ptr + пользовательская delete-функция 

	//Реализовать возможность записи в файл данных (строчек) из разных источников 
	//(для упрощения пусть источниками являются два массива)
	//Так как все "писатели" будут по очереди записывать свои данные в один и тот же файл,
	//логично предоставить им возможность пользоваться одним и тем же указателем FILE* =>
	//безопасной оберткой для такого указателя является shared_ptr 
	//а. Первый владелец должен открыть/создать файл для записи
	//б. Все остальные писатели должны присоединиться к использованию
	//в. Последний владелец указателя должен закрыть файл

	//Подсказка: имитировать порядок записи можно с помощью функции rand()
	{
	//"писатели":
	//Создать writer1, writer2
	shared_ptr<ofstream> writer1(new ofstream("file.txt"));
	auto writer2 = writer1;

	//например, источники данных:
	string str1("Writer1");
	string str2("Writer2");

	//заданное число итераций случайным образом позволяем одному из "писателей" записать в файл
	//свою строчку
	default_random_engine RandEng;
	uniform_int_distribution<size_t> NumberDist{ 1, 10 }; // можно добавить еще рандом для количества писателей

	for (size_t i = 0, count = NumberDist(RandEng); i < count; ++i) {
		//*writer1 << str1 << ' '; // writer1->operator<<(str1); почему не работает?
	}

	for (size_t i = 0, count = NumberDist(RandEng); i < count; ++i) {
		*writer2 << str2 << ' '; 
	}

	__asm nop
	}//закрытие файла??? -  default delete ofstream* -> close();

	return 0;
}
