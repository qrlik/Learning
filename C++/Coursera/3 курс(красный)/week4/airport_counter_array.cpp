
#include <algorithm>
#include <array>
#include <iostream>
#include <random>

using namespace std;

// TAirport should be enum with sequential items and last item TAirport::Last_
template <typename TAirport> class AirportCounter
{
public:
	// конструктор по умолчанию: список элементов пока пуст
	AirportCounter() : airports({ 0 }) {} // инициализация нулями по правилу неполной инициализации

	// конструктор от диапазона элементов типа TAirport
	template <typename TIterator> AirportCounter(TIterator begin, TIterator end) : AirportCounter()
	{
		for (; begin != end; ++begin)
		{
			Insert(*begin); // добавляем вылеты
		}
	}

	// получить количество элементов, равных данному
	size_t Get(TAirport airport) const
	{
		return airports[static_cast<uint32_t>(airport)];
	}

	// добавить данный элемент
	void Insert(TAirport airport)
	{
		++airports[static_cast<uint32_t>(airport)];
	}

	// удалить одно вхождение данного элемента
	void EraseOne(TAirport airport)
	{
		airports[static_cast<uint32_t>(airport)] = (airports[static_cast<uint32_t>(airport)] == 0) ? 0 : airports[static_cast<uint32_t>(airport)] - 1;
	}

	// удалить все вхождения данного элемента
	void EraseAll(TAirport airport)
	{
		airports[static_cast<uint32_t>(airport)] = 0;
	}

	static const size_t SIZE = static_cast<size_t>(TAirport::Last_);
	using Item = pair<TAirport, size_t>;
	using Items = array<Item, static_cast<uint32_t>(TAirport::Last_)>;

	// получить некоторый объект, по которому можно проитерироваться,
	// получив набор объектов типа Item - пар (аэропорт, количество),
	// упорядоченных по аэропорту
	Items GetItems() const
	{
		Items result;
		for (size_t i = 0; i < airports.size(); ++i)
		{
			result[i] = { static_cast<TAirport>(i), airports[i] };
		}
		return result;
	}

private:
	array<size_t, SIZE> airports;
};
