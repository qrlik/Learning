#include "thread_safe_test.h"

#include <iostream>

using namespace std;

void TestThreadsafe_Stack() {
	threadsafe_stack<int> test;
	vector<future<void>> providers;
	const size_t threads = 3;
	const size_t steps = 10;

	for (size_t i = 0; i < threads; ++i) {
		providers.push_back(async(launch::async, TS_provider<int>, ref(test), steps, 150ms)); // ~30 push for ~2s
	}

	size_t pop_count = 0;
	try { // пытаемся обогнать поставки и опустошить стэк
		int buffer = 0;
		while (true) {
			this_thread::sleep_for(25ms);
			test.pop(buffer);
			++pop_count;
		}
	}
	catch (const out_of_range&) {
		cout << "Successful pops : " << pop_count << "\n";
	}

	for (auto& f : providers) {
		//if (!f.valid()) {
		//	throw future_error(future_errc::no_state);
		//}
		f.get();
	}
	cout << "Final count : " << test.size() << " / " << threads * steps << '\n';
	_asm nop;
}
