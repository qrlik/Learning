#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool
{
	set<T*> allocated_set; // множество указателей на выделенные объекты
	queue<T*> free_que; // очередь указателей для свободных объектов
public:
	T* TryAllocate() // попытка взять свободный объект
	{
		if (free_que.empty())
		{ // если нет свободных возвращаем null
			return nullptr;
		} // иначе возвращаем первый свободный в очереди
		T* allocate_ptr = free_que.front(); // забираем и удаляем из очереди свободных
		free_que.pop();
		allocated_set.insert(allocate_ptr); // заносим в множество выделенных объектов
		return allocate_ptr;
	}

	T* Allocate() // выделение объекта
	{
		T* allocate_ptr = TryAllocate(); // пробуем взять из свободных объектов
		if (!allocate_ptr) // если свободных нет, выделяем память и добавляем в множество
		{
			allocate_ptr = new T();
			allocated_set.insert(allocate_ptr);
		}
		return allocate_ptr;
	}

	void Deallocate(T* object) // освобождение объекта
	{
		auto search_obj = allocated_set.find(object); // ищем объект в множестве выделенных
		if (search_obj == allocated_set.end())
		{
			throw invalid_argument("Wrong object to deallocate");
		}
		free_que.push(*search_obj); // заносим в очередь освобожденных
		allocated_set.erase(search_obj); // удаляем из множества выделенных
	}

	~ObjectPool()
	{
		for (auto ptr : allocated_set)
		{
			delete ptr; // освобождаем выделенные объекты
		}

		while (!free_que.empty())
		{
			delete free_que.front(); // освобождаем свободные объекты
			free_que.pop();
		}
	}
};

void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestObjectPool);
	return 0;
}
