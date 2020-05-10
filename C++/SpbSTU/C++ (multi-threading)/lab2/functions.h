#pragma once
#include "profile.h"

#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <cassert>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <functional>
#include <condition_variable>

// Задание 1.
void Task_one_marks();

// Задание 2.
class A {
	thread_local static size_t _counter;
public:
	static size_t getCounter() noexcept;
	A() noexcept;
};

void Task_two_func();

// Задание 3.
template<typename C, typename T>
T MultiAccumulate(const C& cont, size_t thread_amount, T init) {
	assert(thread_amount != 0);
	const size_t range = std::size(cont) / thread_amount;

	std::vector<std::future<T>> futures;
	auto itb = std::cbegin(cont);
	if (range) {
		futures.reserve(thread_amount - 1);
		for (size_t i = 0, size = futures.capacity(); i < size; ++i) {
			auto step_end = std::next(itb, range);
			futures.push_back(std::async(std::launch::async, std::accumulate<decltype(itb), T>, itb, step_end, T{}));
			itb = step_end;
		}
	}
	T summ = std::accumulate(itb, std::cend(cont), T{});

	for (auto& f : futures) {
		//if (!f.valid()) {
		//	throw future_error(future_errc::no_state);
		//}
		summ += f.get();
	}

	return summ;
}

void Task_three_accumulate();

// Задание 4.
void Task_four_exceptions();

// Задание 5.
class MySingleton { // 1 object for all
	MySingleton();
	MySingleton(const MySingleton&) = delete;
	MySingleton& operator=(const MySingleton&) = delete;
public:
	static const MySingleton& getInstance();
};

class MyThreadSingleton { // 1 object for thread
	MyThreadSingleton();
	MyThreadSingleton(const MyThreadSingleton&) = delete;
	MyThreadSingleton& operator=(const MyThreadSingleton&) = delete;
public:
	static const MyThreadSingleton& getInstance();
};

// Задание 6.
class thread_pool {
	std::mutex _work_mutex; //for _threads and _stop_flag
	std::mutex _tasks_mutex;

	std::condition_variable _cv;
	std::queue<std::function<void()>> _tasks;
	std::vector<std::thread> _threads;
	std::vector<size_t> _counters;
	
	bool _stop_flag;
	void _thread_task(size_t);
public:
	thread_pool();
	thread_pool(const thread_pool&) = delete;
	thread_pool& operator=(const thread_pool&) = delete;
	~thread_pool();
	void add_task(std::function<void()>);
	void stop_work();
	bool start_work();
};

void Test_thread_pool();
