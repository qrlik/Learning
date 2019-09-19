//Исключения

//Шаблоны: 
	//		шаблоны функций,
	//		шаблоны классов,

#include <tchar.h>
#include "header.h"
#include "MyString.h"
//#include <stdexcept>

#define stop __asm nop

int _tmain(int argc, _TCHAR* argv[])
{
////////////////////////////////////////////////////////////////////////////
	//Тема. Шаблоны функций.
	//Создайте шаблон функции перестановки местами двух
	//значений - Swap(). Проверьте работоспособность созданного
	//шаблона с помощью приведенного ниже фрагмента кода.
	//Подсказка 1: объявление шаблона корректнее поместить в .h-файл.

	int iX = 1, iY = -1;
	Swap(iX, iY);
	stop

	double dX = 0.5, dY = -5.5;
	Swap(dX, dY);
	stop

	//Подсказка 2: подумайте, что нужно реализовать, для того,
	//			чтобы следующий вызов работал с объектами MyString
	//			не только корректно, но и эффективно
	MyString str1("one"), str2("two");
	Swap(str1, str2); // move operator=
	stop

/////////////////////////////////////////////////////////////////////

	//Тема. Шаблоны классов.
	//Задание 1.
	//Создайте шаблон класса MyStack для хранения элементов любого типа T.
	//Подсказка: 1.элементы нужно где-то хранить - простым и удобным средством
	//			для хранения последовательно расположенных в памяти элементов
	//			является массив, 
	//			2.для задания максимального размера стека может быть использован
	//			параметр-константа шаблона
	//			3.обязательными операциями со стеком являются "push" и "pop". Для
	//			того, чтобы гарантировать корректное выполнение этих операций 
	//			хорошо было бы генерировать исключение в аварийной ситуации
	//			4. дополнительно реализуйте operator[] таким образом, чтобы пользователь
	//			мог читать/изменять значения только тех элементов, которые он формировал


	//С помощью шаблона MyStack создайте стек из 5 элементов int - iStack и
	//стек из 10 элементов MyString - strStack и поэкспериментируйте с функциями
	//push() и pop(), operator[]

	MyStack<int, 5> iStack;
	MyStack<MyString, 10> strStack;
	try
	{
		strStack.push("first");
		std::cout << strStack[0] << std::endl; // first
		iStack.push(4.34);
		std::cout << iStack[0] << std::endl; // 4
		strStack.pop(); 
		std::cout << strStack[0] << std::endl; // Out of range
	} catch (std::out_of_range& er)
	{
		std::cout << er.what() << std::endl;
	}

	stop

	//Задание 2. Реализуйте шаблон стека - MyStack2 таким образом, чтобы 
	//для хранения элементов использовался ОДНОСВЯЗНЫЙ список.
	//Реализуйте возможность распечатать элементы стека в том порядке, в котором их заносил (push())
	//пользователь

	MyStack2<int> iStack2;
	MyStack2<MyString> strStack2;
	try
	{
		strStack2.push("first");
		std::cout << strStack2[0] << std::endl; // first
		iStack2.push(4.34);
		std::cout << iStack2[0] << std::endl; // 4
		strStack2.push("second");
		strStack2.print(); // first second
		strStack2.pop();
		std::cout << strStack2[1] << std::endl; // Out of range
	}
	catch (std::out_of_range& er)
	{
		std::cout << er.what() << std::endl;
	}
	stop

	//Задание 3. Реализуйте шаблон очереди - MyQueue таким образом, чтобы 
	//для хранения элементов использовался динамический массив.
	//При использовании массива следует учесть специфику очереди, то есть
	//когда заполнен "хвост", в "голове" скорее всего уже имеются свободные элементы
	//=> должен быть организован кольцевой буфер

	MyQueue<int> iStack3;
	MyQueue<MyString> strStack3;
	try
	{
		strStack3.push("first"); // first
		std::cout << strStack3[0] << std::endl;
		iStack3.push(4.34);
		std::cout << iStack3[0] << std::endl; // 4
		strStack3.push("second");
		strStack3.push("third"); // first second third
		strStack3.pop(); // second third
		strStack3.push("fourth");
		strStack3.push("fifth");
		strStack3.push("sixth");
		strStack3.push("seventh"); // second third fourth fifth sixth seventh
		std::cout << strStack3[1] << std::endl; // third
	}
	catch (std::exception& er)
	{
		std::cout << er.what() << std::endl;
	}

	stop
	return 0;
}

