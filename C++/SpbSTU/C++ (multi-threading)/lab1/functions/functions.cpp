#include "functions.h"

#include <fstream>
#include <iterator>
#include <chrono>

#include <Windows.h>

using namespace std;

namespace {
	//Задание 1.
	void FileTransform(const string& str) {
		ifstream file(path + str);
		ofstream new_file(path + '_' + str);

		transform(istreambuf_iterator<char>{file}, istreambuf_iterator<char>{},
			ostreambuf_iterator<char>(new_file), [](char letter) {
				return static_cast<char>(toupper(static_cast<unsigned char>(letter))); });
	}

	template<typename It>
	void FileSpecTransformThread(It begin, It end) {
		for (; begin != end; ++begin) {
			FileTransform(*begin);
		}
	}
}

//Задание 1.
void FileSpecTransform(size_t thread_amount) {
	vector<thread> threads;

	const size_t share_count = size(filespec) / thread_amount;
	size_t tail_count = size(filespec) % thread_amount;

	threads.reserve((share_count) ? thread_amount : tail_count);

	for (auto itb = begin(filespec), ite = end(filespec); itb != ite; ) {
		size_t part_size = share_count;

		if (tail_count) {
			++part_size;
			--tail_count;
		}

		auto step_end = next(itb, part_size);
		threads.emplace_back(FileSpecTransformThread<decltype(itb)>, itb, step_end);
		itb = step_end;
	}

	for (auto& th : threads) {
		th.join();
	}
}

//Задание 2.
namespace {
	void SoundNote(size_t frequency, size_t interval) {
		Beep(frequency, interval);
	}

	void Syncope() {
		Beep(294, 1000 / 8);
		Beep(440, 1000 / 4);
		Beep(262 * 2, 1000 / 4);
		Beep(330 * 2, 1000 / 4);
		Beep(415, 1000 / 8);
		Beep(440, 1000);
	}
}

void GammaSound() {
	size_t interval = 500;

	for (auto note : notes) {
		thread th(SoundNote, sound_frequency.at(note), interval);
		th.join();
		//this_thread::sleep_for(15ms);
	}

	this_thread::sleep_for(chrono::milliseconds(interval * notes.size()));
}

void SyncopeSound() {
	thread th(Syncope);
	th.detach();
	this_thread::sleep_for(2s);
}

//Задание 3.
void AlphabetBoost() {
	auto step = chrono::milliseconds(50);

	for (char first = 'A', last = 'Z'; first <= last; ++first) {
		cout << first << ' ';
		this_thread::sleep_for(step * (last - first));
	}
}
