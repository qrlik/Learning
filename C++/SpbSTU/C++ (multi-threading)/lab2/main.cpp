#include "fuctions.h"
using namespace std;

int main(int argc, char* argv[]) {
	// Задание 1. Параллельные алгоритмы std
	Task_one_marks();
	// 5*10^6 - standard(seq) ~80ms , par ~12ms, par_unseq ~8ms
	// 1*10^6 - standard(seq) ~15ms , par ~5ms, par_unseq ~1ms

	//Задание 2. thread_local, future, async
	Task_two_func();
	//#1 with[4] objects
	//#2 with[3] objects
	//#5 with[3] objects
	//#4 with[3] objects
	//#3 with[3] objects

	//Задание 3. Асинхронный запуск потоков.Получение отложенного результата.Параллельная реализация std::accumulate()
	Task_three_accumulate();
	//Standard accumulate : 269 ms
	//My - parallel 16threads accumulate : 42 ms
	//My - parallel 4threads accumulate : 73 ms

	//Задание 4. Обработка исключений посредством future
	//Task_four_exceptions();

	//Задание 5. Thread - safe Singleton

	//Задание 6. Пул потоков.std::thread, std::mutex, std::conditional_variable
	Test_thread_pool();
	//Thread #3 completed 208 tasks
	//Thread #1 completed 217 tasks
	//Thread #11 completed 218 tasks
	//Thread #14 completed 217 tasks
	//Thread #4 completed 218 tasks
	//Thread #8 completed 219 tasks
	//Thread #13 completed 222 tasks
	//Thread #7 completed 222 tasks
	//Thread #15 completed 201 tasks
	//Thread #12 completed 218 tasks
	//Thread #5 completed 223 tasks
	//Thread #9 completed 203 tasks
	//Thread #10 completed 215 tasks
	//Thread #0 completed 215 tasks
	//Thread #2 completed 216 tasks
	//Thread #6 completed 220 tasks
	//Thread completed 3452 tasks totally
	//1548 tasks stayed incompleted
	//Thread_pool 5000 tasks: 12389 ms
	//Unique threads for 5000 tasks : 23058 ms

	return 0;
}
