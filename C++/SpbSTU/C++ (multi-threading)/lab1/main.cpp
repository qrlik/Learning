#include "functions.h"
#include "profile.h"
#include "thread_safe_test.h"

#include <chrono>
#include <numeric>

using namespace std;

int main(int argc, char* argv[]) {

	//Задание 1. Запуск потоков с ожиданием завершения
	//Обработку каждого файла требуется сделать в отдельном потоке.
	//Каждый поток должен прочитать содержимое файла, перевести все буквы в верхний регистр
	//(а цифры и другие символы оставить прежними) и записать в новый файл.
	cout << " First task\n";
	{
		LOG_DURATION("Seq FileSpecTransform");
		FileSpecTransform(1);
	}
	//Seq FileSpecTransform : 36 ms
	{
		LOG_DURATION("Multi-thread FileSpecTransform - " + size(filespec));
		FileSpecTransform(size(filespec));
	}
	//eSpecTransform - : 10 ms
	{
		LOG_DURATION("Multi-thread FileSpecTransform - " + std::thread::hardware_concurrency());
		FileSpecTransform(size(filespec));
	}
	//eSpecTransform - : 13 ms
	cout << '\n';

	//Задание 2. Запуск обособленных потоков.Блокировка потока на заданный интервал времени
	GammaSound();
	SyncopeSound();

	//Задание 3. this_thread
	//В первичном потоке выведите на экран алфавит(‘A’ – ‘Z’) таким образом, чтобы  скорость вывода плавно увеличивалась
	AlphabetBoost();
	cout << '\n';

	//Задание 4. Параллельная реализация std::transform(). Определение числа вычислительных ядер. Измерение времени.
	vector<int> v(1'000'000);
	vector<int> v2(v.size());

	cout << "Fourth task\n";
	iota(v.begin(), v.end(), -1'000'000);
	{
		auto start = chrono::steady_clock::now();
		UserAbsTransform(v, v2, 1);
		auto finish = chrono::steady_clock::now();
		cout << "1 thread Abs " <<
			duration_cast<chrono::microseconds>(finish - start).count() << '\n'; // 15526
	}
	//1 thread Abs 19671
	iota(v.begin(), v.end(), -1'000'000);
	{
		auto start = chrono::steady_clock::now();
		UserAbsTransform(v, v2, 4);
		auto finish = chrono::steady_clock::now();
		cout << "4 thread Abs " <<
			duration_cast<chrono::microseconds>(finish - start).count() << '\n'; // 7075
	}
	//4 thread Abs 5274
	iota(v.begin(), v.end(), -1'000'000);
	{
		auto start = chrono::steady_clock::now();
		UserAbsTransform(v, v2, 16);
		auto finish = chrono::steady_clock::now();
		cout << "16 thread Abs " <<
			duration_cast<chrono::microseconds>(finish - start).count() << '\n'; // 7918
	}
	//16 thread Abs 7159
	cout << '\n';

	//Задание 5. Потокобезопасный стек.std::mutex.std::lock_guard
	cout << "Fifth task\n";
	TestThreadsafe_Stack();
	//Successful pops : 3
	//Final count : 27 / 30
	return 0;
}
