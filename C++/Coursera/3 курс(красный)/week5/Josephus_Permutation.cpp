#include <iterator>
#include <list>

using namespace std;

template <typename Cont, typename ForwardIt> ForwardIt LoopIt(Cont& c, ForwardIt it) // функция позволяющая зациклить список
{
	return (it == c.end()) ? c.begin() : it;
}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
	list<typename RandomIt::value_type> pool;
	for (auto it = first; it != last; ++it)
	{ // Заполняем список перемещением данных
		pool.push_back(move(*it));
	}

	auto it = pool.begin();
	while (!pool.empty())
	{
		*(first++) = move(*it); // переносим обратно данные
		it = pool.erase(it);
		if (pool.empty())
		{
			break;
		}
		it = LoopIt(pool, it); // обновляем итератор
		for (size_t i = 1; i < step_size; i++) // делаем step_size - 1 шагов до следующего элемента
		{
			it = LoopIt(pool, next(it));
		}
	}
}
