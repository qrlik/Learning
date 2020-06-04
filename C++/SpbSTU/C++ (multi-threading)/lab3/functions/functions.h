#pragma once
#include <atomic>
#include <mutex>

// Задание 1.
void Task_one();

// Задание 2.
class my_mutex {
	std::atomic_flag f;
public:
	void lock();
	void unlock();
	bool try_lock();
};

//Задание 3.
template<typename T>
class A_mutex {
	T _a;
	static std::mutex _m;
	static size_t _count;
public:
	A_mutex() : _a() {
		std::lock_guard<std::mutex> lg(_m);
		++_count;
	}
	~A_mutex() {
		std::lock_guard<std::mutex> lg(_m);
		--_count;
	}
	static size_t getCount() {
		std::lock_guard<std::mutex> lg(_m);
		return _count;
	}
};

template<typename T>
class A_my_mutex {
	T _a;
	static my_mutex _m;
	static size_t _count;
public:
	A_my_mutex() : _a() {
		std::lock_guard<my_mutex> lg(_m);
		++_count;
	}
	~A_my_mutex() {
		std::lock_guard<my_mutex> lg(_m);
		--_count;
	}
	static size_t getCount() {
		std::lock_guard<my_mutex> lg(_m);
		return _count;
	}
};

template<typename T>
class A_atomic {
	T _a;
	static std::atomic<size_t> _count;
public:
	A_atomic() : _a() {
		++_count;
	}
	~A_atomic() {
		--_count;
	}
	static size_t getCount() {
		return _count;
	}
};

void Task_three();

//Задание 4.
void Task_four();
