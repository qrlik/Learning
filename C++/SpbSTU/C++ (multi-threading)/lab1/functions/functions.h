#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <thread>
#include <optional>
#include <cassert>

const std::string path = { "1task_files/" };
const std::string filespec[] = { "1.txt", "2.txt", "3.txt", "4.txt",
								"5.txt", "6.txt", "7.txt", "8.txt",
								"9.txt", "10.txt", "11.txt", "12.txt", 
								"13.txt", "14.txt", "15.txt", "16.txt" };

const std::vector<char> notes = { 'C', 'D', 'E', 'F', 'G', 'A', 'B' };
const std::unordered_map<char, size_t> sound_frequency =
								{	{ 'C', 261 },	//  ДО
									{ 'D', 293 },	//  РЕ
									{ 'E', 329 },	//  МИ
									{ 'F', 349 },	//  ФА
									{ 'G', 392 },	//  СОЛЬ
									{ 'A', 440 },	//  ЛЯ
									{ 'B', 493 } };	//  СИ

//Задание 1.
void FileSpecTransform(size_t thread_amount = std::thread::hardware_concurrency());

//Задание 2.
void GammaSound();
void SyncopeSound();

//Задание 3.
void AlphabetBoost();

//Задание 4.
namespace {
	template<typename It1, typename It2>
	void AbsTransformThread(It1 begin, It1 end, It2 d_first) {
		std::transform(begin, end, d_first, [](std::decay_t<decltype(*begin)> value) {
				return std::abs(value);
			});
	}

	template<typename Cont1, typename Cont2>
	void AbsTransform(const Cont1& source, Cont2& dest, size_t thread_amount) {
		assert(thread_amount <= std::size(source));

		const size_t step_amount = std::size(source) / thread_amount;
		auto itb = std::begin(source);
		auto d_itb = std::begin(dest);

		std::vector<std::thread> threads;
		threads.reserve(thread_amount - 1);
		for (size_t th = 0; th + 1 < thread_amount; ++th) {
			auto step_end = std::next(itb, step_amount);
			threads.emplace_back(AbsTransformThread<decltype(itb), decltype(d_itb)>,
				itb, step_end, d_itb); // for list O(2N)
			itb = step_end;
			std::advance(d_itb, step_amount);
		}
		AbsTransformThread(itb, std::end(source), d_itb); // share_count + tail

		for (auto& th : threads) {
			th.join();
		}
	}
}

template<typename Cont1, typename Cont2>
void UserAbsTransform(const Cont1& source, Cont2& dest, std::optional<size_t> th_amount = {}) {
	const size_t source_size = std::size(source);
	if (!source_size) {
		return;
	}

	if (!th_amount) {
		std::cout << "Supported number of concurrent threads - " << std::thread::hardware_concurrency() << '\n'
			<< "Enter number of threads to work less or equal than " << source_size << '\n';

		th_amount = [source_size]() {
			int result = 0;
			while (std::cin >> result && (result < 1 || static_cast<size_t>(result) > source_size)) {
				std::cout << "Incorrent number, 1 <= x <= source_size is necessary" << '\n';
			}
			if (result == 0) { // first cin -> false
				result = 1;
			}
			return static_cast<size_t>(result);
		}(); // thread_amount > 1;
	}

	AbsTransform(source, dest, th_amount.value());
}
