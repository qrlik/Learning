#include "functions.h"
#include "profile.h"
#include "queue.h"

#include <algorithm>
#include <cassert>

using namespace std;

// Задание 1.
namespace {
	struct sync_vector {
		vector<int> v;
		mutex m;
	};

	template <typename int sign>
	void Task_one_push(promise<void> p, sync_vector& data, size_t count) {
		{
			lock_guard<mutex> lg(data.m);
			for (size_t i = 0; i < count; ++i) {
				data.v.push_back(static_cast<int>(i) * sign);
			}
		}
		p.set_value();
	}

	template<typename Func>
	void Task_one_call(future<void> f1, future<void> f2, sync_vector& v, Func f) {
		f1.wait();
		f2.wait();
		f(v);
	}
}

void Task_one() {
	size_t count = 10;
	sync_vector data;
	data.v.reserve(count * 2);

	promise<void> p1;
	promise<void> p2;
	future<void> f1 = p1.get_future();
	future<void> f2 = p2.get_future();
	// Запустите параллельно два (несколько) detach - ed  (независимых) потока.
	// Каждый поток будет добавлять  в вектор заданное количество значений
	thread th1(Task_one_push<1>, move(p1), ref(data), count);
	th1.detach();
	thread th2(Task_one_push<-1>, move(p2), ref(data), count);
	th2.detach();

	auto f = [](sync_vector& data) {
		transform(data.v.begin(), data.v.end(), data.v.begin(), [](int value) {return abs(value); });
	};
	// Запустите еще один поток, который должен окончательно сформированный предыдущими потоками вектор
	//преобразовать согласно заданному Вами условию 
	thread th3(Task_one_call<decltype(f)>, move(f1), move(f2), ref(data), f);
	th3.join();
	_asm nop;
}

// Задание 2.
void my_mutex::lock() {
	//while (f.test_and_set(std::memory_order_acquire)) {};
	while (f.test_and_set()) {};
}

void my_mutex::unlock() {
	//f.clear(std::memory_order_release);
	f.clear();
}

bool my_mutex::try_lock() {
	//return !f.test_and_set(std::memory_order_acquire);
	return !f.test_and_set();
}

//Задание 3.
template<typename T>
mutex A_mutex<T>::_m{};
template<typename T>
size_t A_mutex<T>::_count = 0;

template<typename T>
my_mutex A_my_mutex<T>::_m{};
template<typename T>
size_t A_my_mutex<T>::_count = 0;

template<typename T>
std::atomic<size_t> A_atomic<T>::_count = 0;

namespace {
	template<typename A>
	void A_test() {
		vector<future<void>> futures;
		futures.reserve(16);
		{
			LOG_DURATION("A_mutex");
			for (size_t i = 0; i < 16; ++i) {
				futures.push_back(async(launch::async, []() {
					for (size_t i = 0; i < 100'000; ++i) {
						A tmp;
					}}));
			}
			for (auto& f : futures) {
				f.wait();
			}
		}
		assert(A::getCount() == 0);
	}
}

void Task_three() {
	A_test<A_mutex<int>>();
	A_test<A_my_mutex<int>>();
	A_test<A_atomic<int>>();
}

//Задание 4.

namespace {
	template<size_t capacity>
	void Writers_thread(Queue<capacity>& q) {
		for (char b = 'A'; b <= 'Z'; ++b) {
			q.write(b);
		}
	}
}

void Task_four() {
	const size_t cap = 100;
	Queue<cap> q;
	vector<future<void>> futures;
	futures.reserve(8);

	for (size_t i = 0; i < 8; ++i) { // 8 times 26 writes 208
		futures.push_back(async(launch::async, Writers_thread<cap>, ref(q)));
	}

	for (size_t i = 0; i < 25; ++i) { //25
		q.read();
	}

	for (auto& f : futures) {
		//if (!f.valid()) {
		//	throw future_error(future_errc::no_state);
		//}
		f.wait();
	}
	
	_asm nop
}
