#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <map>
#include <set>
#include <utility>

using namespace std;


template <typename T> class PriorityCollection
{
public:
	using Id = size_t;
	using Priority = size_t;
private:
	struct Data
	{
		T _data;
		Priority _priority;
	};

	// Приватные поля и методы
	size_t id_count;
	map<Priority, set<Id>> collection; // сортировка по приоритету и по добавлению
	map<Id, Data> objects; // key - id, value - данные T и Priority(для метода Promote)
public:
	PriorityCollection() : id_count(0) {}
	Id Add(T object)
	{ // Добавить объект с нулевым приоритетом с помощью перемещения и вернуть его идентификатор
		objects[id_count] = { move(object), 0 }; 
		collection[0].insert(id_count);
		return id_count++; // возвращаем индекс и увеличиваем счетчик
	}

	template <typename ObjInputIt, typename IdOutputIt> void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin)
	{ // Добавить все элементы диапазона [range_begin, range_end) с помощью перемещения, записав выданные им идентификаторы в диапазон [ids_begin, ...)
		for (; range_begin != range_end; ++range_begin)
		{
			*ids_begin++ = Add(move(*range_begin));
		}
	}

	bool IsValid(Id id) const
	{ // Определить, принадлежит ли идентификатор какому-либо хранящемуся в контейнере объекту
		return objects.count(id) == 1;
	}

	// Методы Get и Promote всегда вызываются от валидных с точки зрения метода IsValid идентификаторов
	const T& Get(Id id) const
	{ // Получить объект по идентификатору
		return objects.at(id)._data;
	}

	void Promote(Id id)
	{ // Увеличить приоритет объекта на 1
		Priority& pr = objects[id]._priority; // старый приоритет
		collection[pr].erase(id); // удалили из старого множества
		collection[++pr].insert(id); // добавили в новое и увеличили приоритет
	}

	// Методы GetMax и PopMax вызываются только при наличии элементов в контейнере
	pair<const T&, int> GetMax() const
	{ // Получить объект с максимальным приоритетом и его приоритет (если таких несколько, получить самый свежий)
		const Data& result = objects.at(*collection.rbegin()->second.rbegin()); // у наивысшего приоритета, взяли последний индекс во множестве и по нему получили данные в objects
		return { result._data, result._priority };
	}

	// Аналогично GetMax, но удаляет элемент из контейнера
	pair<T, int> PopMax()
	{
		Id index = *collection.rbegin()->second.rbegin(); // индекс максимального объекта
		Data& max_data = objects[index]; // ссылка на данные
		pair<T, int> result_data(move(max_data._data), max_data._priority); // переносим в локальную переменную

		collection[max_data._priority].erase(index);
		while (!collection.empty() && collection.rbegin()->second.empty())
		{ // т.к после удаления максимума может возникнуть хвост из пустных множеств, нужно удалить лишние ключи
			collection.erase(--collection.end());
		}

		objects.erase(index);
		return result_data;
	}
};
