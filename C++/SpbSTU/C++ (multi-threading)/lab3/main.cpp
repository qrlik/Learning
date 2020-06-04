#include "functions.h"
#include "thread_safe_test.h"

using namespace std;

int main(int argc, char* argv[]) {
	// Задание 1. std::future<>, std::promise<>
	Task_one();

	// Задание 2. Реализация spinlock. std::atomic_flag
	TestThreadsafe_Stack();

	//	Successful pops : 18
	//	Threadsafe stack with shared mutex : 14554 ms
	//	Final count : 282 / 300

	//	Successful pops : 18
	//	Threadsafe stack with my_mutex : 14552 ms
	//	Final count : 282 / 300

	//Задание 3. Атомарные типы данных.
	Task_three();
	//A_mutex: 1034 ms
	//A_my_mutex : 1426 ms
	//A_atomic : 47 ms

	//Задание 4. Потокобезопасная кольцевая очередь фиксированного размера на базе условных переменных. std::condition_variable::wait_for()
	Task_four();

	//abcdefgh
	//Destruction:208 writers with 8 dones, 25 readers with 8 dones.

	//abcdefghijkabcdefghijklmn
	//Destruction : 208 writers with 125 dones, 25 readers with 25 dones.

	//abcdefgabcdefgh
	//Destruction : 208 writers with 15 dones, 25 readers with 15 dones.
	return 0;
}
