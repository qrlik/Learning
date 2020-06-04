#pragma once
#include "threadsafe_stack.h"

#include <future>

//Поставщик
template<typename T>
void TS_provider(threadsafe_stack<T>& source, size_t steps, std::chrono::milliseconds interval) {
	for (size_t i = 0; i < steps; ++i) {
		source.emplace();
		std::this_thread::sleep_for(interval);
	}
}

void TestThreadsafe_Stack();
