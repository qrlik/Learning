#include "fuctions.h"

#include <algorithm>
#include <execution>
#include <sstream>

using namespace std;

// Задание 1.
void Task_one_marks() {
	vector<int> v(1'000'000);
	cout << "First task - increment marks\n";
	{
		LOG_DURATION("Standard transform")
			transform(v.begin(), v.end(), v.begin(), [](int value)
				{ return ++value; });
	}

	{
		LOG_DURATION("Sequenced transform")
			transform(execution::seq, v.begin(), v.end(), v.begin(), [](int value)
				{ return ++value; });
	}

	{
		LOG_DURATION("Parallel transform")
			transform(execution::par, v.begin(), v.end(), v.begin(), [](int value)
				{ return ++value; });
	}

	{
		LOG_DURATION("Parallel unsequenced transform")
			transform(execution::par_unseq, v.begin(), v.end(), v.begin(), [](int value)
				{ return ++value; });
	}
	cout << '\n';
}

// Задание 2.
thread_local size_t A::_counter = 0;

A::A() noexcept {
	++_counter;
}

size_t A::getCounter() noexcept {
	return _counter;
}

namespace {
	void Task_two_thread_func(size_t number) {
		A a1;
		A* pA = new A();
		thread_local A a2;
		static A a3; // one time in one thread

		delete pA;

		ostringstream os;
		os << "#" << number << " with [" << A::getCounter() << "] objects\n";
		cout << os.str();
	}
}

void Task_two_func() {
	vector<future<void>> futures;

	cout << "Second task - static thread_local\n";
	for (size_t i = 1; i < 6; ++i) {
		futures.push_back(async(launch::async, Task_two_thread_func, i));
	}

	for (auto& f : futures) {
		//if (!f.valid()) {
		//	throw future_error(future_errc::no_state);
		//}
		f.get();
	}

	cout << '\n';
}

// Задание 3.
void Task_three_accumulate() {
	vector<int> v(10'000'000);
	iota(v.begin(), v.end(), 1);

	cout << "Third task - accumulate\n";
	{
		int64_t summ = 0;
		LOG_DURATION("Standard accumulate");
		cout << "Median is " << accumulate(v.begin(), v.end(), summ) / v.size() << '\n';
	}

	{
		int64_t summ = 0;
		LOG_DURATION("My-parallel 16threads accumulate");
		cout << "Median is " << MultiAccumulate(v, std::thread::hardware_concurrency(), summ) / v.size() << '\n';
	}

	{
		int64_t summ = 0;
		LOG_DURATION("My-parallel 4threads accumulate");
		cout << "Median is " << MultiAccumulate(v, 4, summ) / v.size() << '\n';
	}

	{
		int64_t summ = 0;
		LOG_DURATION("My-parallel 32threads accumulate");
		cout << "Median is " << MultiAccumulate(v, 32, summ) / v.size() << '\n';
	}

	cout << '\n';
}

// Задание 4.
namespace {
	void UserIntegerScaner() {
		string user_input;
		size_t pos = 0;
		while (cin >> user_input && !user_input.empty()) {
			int user_digit = stoi(user_input, &pos); // out_of_range / invalid_argument
			if (pos != user_input.size()) { // bad chars
				throw invalid_argument("Bad input");
			}
		}
	}
}
void Task_four_exceptions() {
	cout << "Fourth task - exceptions\n";
	future<void> f = async(launch::async, UserIntegerScaner);

	try {
		//if (!f.valid()) {
		//	throw future_error(future_errc::no_state);
		//}
		f.get();
	}
	catch (const exception& ex) {
		cerr << ex.what() << '\n';
	}
}

// Задание 5.
MySingleton::MySingleton() {}

const MySingleton& MySingleton::getInstance() {
	static const MySingleton _instance; // const for thread-safety
	return _instance;
}
////////////////////////////////////////////////////////////////////////////
MyThreadSingleton::MyThreadSingleton() {}

const MyThreadSingleton& MyThreadSingleton::getInstance() {
	thread_local static const MyThreadSingleton _instance;
	return _instance;
}

// Задание 6.
void thread_pool::_thread_task(size_t th_index){
	while (true) {
		function<void()> task;
		{
			unique_lock ul(_tasks_mutex);

			_cv.wait(ul, [&]() {
				return _stop_flag || !_tasks.empty(); });

			if (_stop_flag) {
				ostringstream os;
				os << "Thread #" << th_index << " completed " << _counters[th_index] << " tasks\n";
				cout << os.str();
				return;
			}

			task = _tasks.front();
			_tasks.pop();
		}
		task();
		++_counters[th_index]; // only this thread access [th_index] / no data-race
	}
}

thread_pool::thread_pool(): _stop_flag(false) {
	const size_t ths_amount = thread::hardware_concurrency();
	_counters.resize(ths_amount);
	_threads.reserve(ths_amount);
	for (size_t i = 0; i < ths_amount; ++i) {
		_threads.emplace_back(&thread_pool::_thread_task, this, i);
	}
}

thread_pool::~thread_pool() {
	stop_work();

	size_t summ = 0;
	for (auto i : _counters) {
		summ += i;
	}
	cout << "Thread completed " << summ << " tasks totally\n";
	cout << _tasks.size() << " tasks stayed incompleted\n";
}

void thread_pool::add_task(std::function<void()> func) {
	lock_guard lg(_tasks_mutex);
	_tasks.push(move(func));
	_cv.notify_one();
}

void thread_pool::stop_work() {
	lock_guard lg(_work_mutex);
	if (_stop_flag) { // если уже был stop_work
		return;
	}
	_stop_flag = true;
	_cv.notify_all();
	for (auto& th : _threads) {
		if (th.joinable()) {
			th.join();
		}
	}
}

bool thread_pool::start_work() {
	lock_guard lg(_work_mutex);
	if (!_stop_flag) {
		return false; // работа уже производится
	}
	_stop_flag = false; // сюда попадем только если _stop_flag был true -> отработал stop_work() ->
	// все потоки были join() -> значит нужно перезапустить все потоки

	for (size_t i = 0, ths = _threads.size(); i < ths; ++i) {
		thread tmp(&thread_pool::_thread_task, this, i);
		_threads[i].swap(tmp);
	}
	return true; // работа возобновлена
}

namespace {
	void Test_task() {
		vector<int64_t> v(1'000'000);
		iota(v.begin(), v.end(), -500'000);
		for (auto& i : v) {
			i = i * i;
		}
	}
}

void Test_thread_pool() {
	cout << "Sixth task - thread_pool\n";
	{
		LOG_DURATION("Thread_pool 5000 tasks");
		thread_pool new_pool;
		for (size_t i = 0; i < 5000; ++i) {
			new_pool.add_task(Test_task);
		}
		_asm nop;
	}

	{
		LOG_DURATION("Unique threads for 5000 tasks");
		vector<thread> threads;
		threads.reserve(5000);
		for (size_t i = 0; i < 5000; ++i) {
			threads.emplace_back(Test_task);
		}

		for (auto& t : threads) {
			t.join();
		}
	}
	cout << '\n';
}
