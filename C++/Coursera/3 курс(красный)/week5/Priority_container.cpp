#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <map>
#include <utility>
#include <vector>
#include <list>

using namespace std;

template <typename T> class PriorityCollection
{
	using Id = size_t;
	using Priority = size_t;

	using Obj_list = list<T>;
	struct Obj_info
	{
		Priority obj_priority;
		typename list<T>::iterator obj_it;
	};

	// Приватные поля и методы
	vector<Obj_list> collection; // приоритет - список элементов
	vector<Obj_info> objects; // id - итератор на элемент
	// после PopMax возникнут места с нулевыми итераторами, куда можно вставлять новые. для хранения таких индексов можно ввести доп. структуру (очередь например)
public:
	PriorityCollection() : collection(1) {} // сразу создадим нулевой приоритет
	Id Add(T object)
	{ // Добавить объект с нулевым приоритетом с помощью перемещения и вернуть его идентификатор
		auto it = collection[0].insert(collection[0].end(), move(object)); // вставляем в конец новый элемент
		objects.push_back({ 0, it }); // вставляем нулевой приоритет и итератор
		return objects.size() - 1; // возвращаем индекс для вектора
	}

	template <typename ObjInputIt, typename IdOutputIt> void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin)
	{ // Добавить все элементы диапазона [range_begin, range_end) с помощью перемещения, записав выданные им идентификаторы в диапазон [ids_begin, ...)
		for (; range_begin != range_end; ++range_begin)
		{
			*ids_begin++ = Add(move(*(range_begin)));
		}
	}

	bool IsValid(Id id) const
	{ // Определить, принадлежит ли идентификатор какому-либо хранящемуся в контейнере объекту
		try
		{
			return objects.at(id).obj_it != nullptr;
		}
		catch (out_of_range& ex)
		{
			return false;
		}
	}

	// Методы Get и Promote всегда вызываются от валидных с точки зрения метода IsValid идентификаторов
	const T& Get(Id id) const
	{ // Получить объект по идентификатору
		return *objects[id].obj_it;
	}

	void Promote(Id id)
	{ // Увеличить приоритет объекта на 1
		Priority& old_priority = objects[id].obj_priority;  // получили текущий приоритет (по ссылке чтобы потом поменять)
		Priority new_priority = old_priority + 1;
		if (collection.size() == new_priority) // если размер вектора заполнен
		{ // добавляем новый приоритет, если такого не было
			collection.emplace_back();
		}
		auto& it = objects[id].obj_it; // получили текущий итератор (по ссылке чтобы потом поменять)
		auto new_it = collection[new_priority].insert(collection[new_priority].end(), move(*it)); // перенесли объект в список
		collection[old_priority].erase(it); // удалили старый (пустой) объект
		it = new_it; // заменили итератор
		++old_priority; // увеличили приоритет
	}

	// Методы GetMax и PopMax вызываются только при наличии элементов в контейнере
	pair<const T&, int> GetMax() const
	{ // Получить объект с максимальным приоритетом и его приоритет
		return { collection.back().back(), collection.size() - 1 };
	}

	// Аналогично GetMax, но удаляет элемент из контейнера
	pair<T, int> PopMax()
	{
		pair<T, int> result(move(collection.back().back()), collection.size() - 1);
		while (collection.back().empty())
		{ // удаляем все последние пустые списки которые могли образоваться
			collection.pop_back();
		}
		return result;
	}
};


class StringNonCopyable : public string {
public:
	using string::string;  // Позволяет использовать конструкторы строки
	StringNonCopyable(const StringNonCopyable&)
	{
		cout << "WTF" << endl;
	}
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
	PriorityCollection<StringNonCopyable> strings;
	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");

	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) {
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestNoCopy);
	return 0;
}
