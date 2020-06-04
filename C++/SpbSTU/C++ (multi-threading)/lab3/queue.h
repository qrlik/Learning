#pragma once
#include <vector>
#include <sstream>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

const std::chrono::seconds timeout{ 3 };

template<size_t capacity>
class Queue {
	std::vector<std::future<void>> _writers;
	std::vector<std::future<void>> _readers;
	char _buf[capacity]{ 0 };

	std::mutex _input_output;
	std::mutex _writers_mutex;
	std::mutex _readers_mutex;

	std::condition_variable _input;
	std::condition_variable _output;

	size_t _writed{};
	size_t _readed{};

	size_t _size{};
	size_t _first{};
	size_t _last{};

	bool _stop_flag{ false };

	void thread_write(char c) {
		std::unique_lock<std::mutex> ul(_input_output);
		_input.wait_for(ul, timeout, [&]() {
			return _stop_flag || _size < capacity; // ждем место либо команду стоп
			});
		/* wait_for return true -> был стоп или место освободилось? */
		/* wait_for return false -> нет стопа и нет места -> был тайм аут*/
		/*
		if(!_input.wait_for(ul, timeout, [&]() {
			return _stop_flag || _size < capacity; // если получили тайм-аут
			})) {
			//_stop_flag = true;
			//_input.notify_all(); _output.notify_all(); // завершаем работу
		}
		if(_stop_flag) { // либо поставил деструктор, либо верхний if
			return; }
		// сюда попадаем только если _size < capacity -> работаем
		*/

		if (_stop_flag || _size == capacity) { // stop_flag || timeout
			return;
		}

		_buf[_last] = c;
		++_writed;

		++_size;
		_last = (_first + _size) % capacity;

		_output.notify_one(); // будим 1 читателя
	}
	void thread_read() {
		char c;
		{
			std::unique_lock<std::mutex> ul(_input_output);
			_output.wait_for(ul, timeout, [&]() {
				return _stop_flag || _size > 0;
				});
			if (_stop_flag || _size == 0) { // stop_flag || timeout
				return;
			}

			c = _buf[_first];
			++_readed;

			--_size;
			_first = (_last + capacity - _size) % capacity;

			_input.notify_one(); // будим 1 писателя
		}
		std::cout << static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	}
public:
	Queue() = default;
	Queue(const Queue&) = delete;
	Queue& operator=(const Queue&) = delete;
	~Queue() {
		_stop_flag = true;

		std::ostringstream os;
		os << "\nDestruction :" << _writers.size() << " writers with " << _writed << " dones, "
			<< _readers.size() << " readers with " << _readed << " dones.\n";
		std::cout << os.str();

		_input.notify_all();
		_output.notify_all();

		for (auto& f : _writers) {
			//if (!f.valid()) {
			//	throw future_error(future_errc::no_state);
			//}
			f.wait();
		}
		for (auto& f : _readers) {
			//if (!f.valid()) {
			//	throw future_error(future_errc::no_state);
			//}
			f.wait();
		}
	}

	void write(char c) {
		std::lock_guard<std::mutex> lg(_writers_mutex);
		_writers.push_back(std::async(std::launch::async, &Queue::thread_write, this, c));
	}
	void read() {
		std::lock_guard<std::mutex> lg(_readers_mutex);
		_readers.push_back(std::async(std::launch::async, &Queue::thread_read, this));
	}
};
