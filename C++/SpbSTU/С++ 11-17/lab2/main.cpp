//Практика 2:

#include <string>
#include <list>
#include <iostream>

#include "unique_vector.h"
#include "my_queue.h"
#include "my_unique_ptr.h"

using namespace std;

int main()
{
	//////////////////////////////////////////////////////////////////////////////////
	//Задание 1. Создайте класс, который должен быть "оберткой" для вектора с УНИКАЛЬНЫМИ значениями
	//любого типа. 
	//Для хранения элементов используйте std::vector

	//Реализуйте конструктор, который может принимать любое количество значений (значения могут повторяться)
	//Реализуйте метод добавления любого количества значений (значения могут повторяться)
	//Реализуйте метод удаления любого количества значений (значения могут повторяться)
	//Реализуйте метод сортировки, который будет принимать в качестве параметра признак по возрастанию / по убыванию

	{
		vector<string> test{ "first", "second", "third", "fourth", "fifth", "first", "second" };
		UniqueVector<string> uniq_1(test);

		UniqueVector<string> uniq_2(move(test));
		uniq_2.Add(vector<string>{ "New_add_str2", "fourth", "fifth", "first", "New_add_str1", "New_add_str2" });
		// second, New_add_str2, first, fourth, third, fifth, New_add_str1
		uniq_2.Delete(vector<string>{ "New_add_str2", "New_add_str1", "New_add_str2", "fifth", "garbage_str" });
		// second, first, fourth, third
		uniq_2.Sort([](const auto& lhs, const auto& rhs) { return lhs < rhs; });
		__asm nop
		const vector<string> result = { "first", "fourth", "second", "third" };
		assert(result == uniq_2.getUniques());
	}

	//Модифицируйте класс из предыдущего задания (или создайте новый) таким образом,
	//чтобы значения не только были бы уникальными, но и находились в заданном диапазоне.
	//Замечание: такой класс в общем виде (для любых данных) смысла, наверно, не имеет, но его можно
	//использовать для таких типов данных, как целых, плавающих и тех пользовательских, для которых
	//имеют смысл операции сравнения (больше, меньше)
	//Подумайте: какие данные нужно добавить в класс и какие методы нужно модифицировать 

	{
		vector<string> test{ "first", "second", "third", "fourth", "fifth", "first", "second" };
		UniqueVector<string> uniq_1(test, pair{ "f", "g" }); // fourth, first, fifth

		UniqueVector<string> uniq_2(move(test), pair{ "f", "t" }); // fourth, first, fifth, second
		uniq_2.Add(vector<string>{ "new_add_str2", "fourth", "fifth", "first", "new_add_str1", "new_add_str2" });
		// second, New_add_str2, first, fourth, third, fifth, new_add_str1, new_add_str2
		uniq_2.Delete(vector<string>{ "New_add_str2", "new_add_str2", "fifth", "garbage_str" });
		// second, first, fourth, new_add_str1
		uniq_2.Sort([](const auto& lhs, const auto& rhs) { return lhs < rhs; });
		const vector<string> result = { "first", "fourth", "new_add_str1", "second" };
		assert(result == uniq_2.getUniques());
		__asm nop
	}

	//Задание 2. Реализуйте шаблонный класс, который является оберткой для очереди с элементами любого типа.
	//	Очередь требуется реализовать посредством динамического массива, при этом использовать массив как циклический буфер.При реализации нужно обеспечить минимальную фрагментацию памяти = >
	//	•	предусмотреть наличие резервных элементов
	//	•	память без очевидной необходимости не перераспределять
	//	Тестируем разработанный класс на приведенном ниже фрагменте.Следующий фрагмент должен работать не только корректно, но и эффективно :

	{
		MyQueue<string> q1{ string("AAA"), string("qwerty")};
		string s("abc");
		q1.push(s);
		q1.push(string("123"));
		string s1 = q1.back();
		assert(s1 == "123");
		q1.push("qqq");
		MyQueue <string> q2 = q1;
		MyQueue <string> q3{ 10, string("!") }; //очередь должна содержать 10 элементов с строкой «!» 
		q1 = q3;
		q2 = MyQueue <string>(5, string(" ? "));
		q1 = MyQueue{ string("bbb"), string("ssss") };

		for (const auto& element : q1) {
			cout << element << '\n';
		}

		__asm nop
	}

	//Задание 3. Реализуйте шаблон класса MyUniquePTR, который является оберткой для указателя на объект любого типа.
	//	Задача – класс должен обеспечивать единоличное владение динамически создаваемым объектом.Проверить функционирование шаблона на примере MyString :

	{
		MyUniquePTR<string> p1(new string("abc"));
		cout << *p1 << '\n';
		p1.set("qwerty");
		string s2 = *p1;
		assert(s2 == "qwerty");

		MyUniquePTR<string> p3(move(p1));
		assert(p1.get() == nullptr);
		if (!p1) { cout << "No object!\n"; }

		vector<MyUniquePTR<string>> v; //как проинициализировать???
		list<MyUniquePTR<string>> l;
		//как скопировать из v в l ???
		__asm nop
	}

	return 0;
}
