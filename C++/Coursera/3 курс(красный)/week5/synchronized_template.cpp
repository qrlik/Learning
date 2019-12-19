#include "test_runner.h"

#include <numeric>
#include <vector>
#include <string>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

// Реализуйте шаблон Synchronized<T>.
// Метод GetAccess должен возвращать структуру, в которой есть поле T& value.
template <typename T> class Synchronized
{
	T value;
	mutex ref_mutex;
public:
	struct Access
	{
		lock_guard<mutex> guard;
		T& ref_to_value;
	};
	explicit Synchronized(T initial = T()) : value(move(initial)) {}

	Access GetAccess()
	{
		return { lock_guard(ref_mutex), value}; // конструируем lock_guard, и в зависимости от ref_mutex поток будет ожидать доступа, 
	} // либо начнет работу
};

void TestConcurrentUpdate() {
	Synchronized<string> common_string; // строка в оболочке

	const size_t add_count = 50000;
	auto updater = [&common_string, add_count] { // 50к раз пробуем прибавить букву
		for (size_t i = 0; i < add_count; ++i) {
			auto access = common_string.GetAccess(); // создается lock_guard для потока. далее либо ожидание, либо прибавляем букву
			access.ref_to_value += 'a';
		} // уничтожается lock_guard и освобождает доступ для других потоков
	};

	auto f1 = async(updater);
	auto f2 = async(updater);

	f1.get();
	f2.get();

	ASSERT_EQUAL(common_string.GetAccess().ref_to_value.size(), 2 * add_count); // 100к
}

vector<int> Consume(Synchronized<deque<int>>& common_queue) {
	vector<int> got;

	for (;;) {
		deque<int> q;

		{
			// Мы специально заключили эти две строчки в операторные скобки, чтобы
			// уменьшить размер критической секции. Поток-потребитель захватывает
			// мьютекс, перемещает всё содержимое общей очереди в свою
			// локальную переменную и отпускает мьютекс. После этого он обрабатывает
			// объекты в очереди за пределами критической секции, позволяя
			// потоку-производителю параллельно помещать в очередь новые объекты.
			//
			// Размер критической секции существенно влияет на быстродействие
			// многопоточных программ.
			auto access = common_queue.GetAccess();
			q = move(access.ref_to_value);
		}

		for (int item : q) {
			if (item > 0) {
				got.push_back(item);
			}
			else {
				return got;
			}
		}
	}
}

void TestProducerConsumer() {
	Synchronized<deque<int>> common_queue;

	auto consumer = async(Consume, ref(common_queue));

	const size_t item_count = 100000;
	for (size_t i = 1; i <= item_count; ++i) {
		common_queue.GetAccess().ref_to_value.push_back(i);
	}
	common_queue.GetAccess().ref_to_value.push_back(-1);

	vector<int> expected(item_count);
	iota(begin(expected), end(expected), 1);
	ASSERT_EQUAL(consumer.get(), expected);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestConcurrentUpdate);
	RUN_TEST(tr, TestProducerConsumer);
}
