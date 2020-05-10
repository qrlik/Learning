#pragma once
#include <vector>
#include <shared_mutex>
//Задание 5.
template <typename T>
class threadsafe_stack {
	std::vector<T> _data;
	mutable std::shared_mutex _mutex;

public:
	threadsafe_stack() = default;
	threadsafe_stack(const threadsafe_stack& other) {
		std::shared_lock sl(other._mutex);
		_data = other._data;
	}
	threadsafe_stack& operator=(const threadsafe_stack& other) {
		if (this != &other) {
			std::lock_guard lg(_mutex, std::defer_lock);
			std::shared_lock sl(other._mutex, std::defer_lock);
			std::lock(lg, sl);
			_data = other._data;
		}
		return *this;
	}
	threadsafe_stack(threadsafe_stack&& other) {
		std::lock_guard lg(other._mutex);
		_data = std::move(other._data);
	}
	threadsafe_stack& operator=(threadsafe_stack&& other) {
		if (this != &other) {
			std::scoped_lock sl(_mutex, other._mutex);
			_data = std::move(other._data);
		}
		return *this;
	}
	~threadsafe_stack() = default;

	bool empty() const {
		std::shared_lock sl(_mutex);
		return _data.empty();
	}

	size_t size() const {
		std::shared_lock sl(_mutex);
		return _data.size();
	}

	void pop(T& value) {
		std::lock_guard	lg(_mutex);
		if (_data.empty()) {
			throw std::out_of_range("threadsafe_stack is empty!");
		}
		value = std::move(_data.back()); // if exception here -> no pop_back
		_data.pop_back();
	}

	void push(T value) {
		std::lock_guard lg(_mutex);
		_data.push_back(std::move(value));
	}

	template<typename... Args>
	void emplace(Args&&... args) {
		std::lock_guard lg(_mutex);
		_data.emplace_back(std::forward(args)...);
	}

	void swap(threadsafe_stack& other) {
		std::scoped_lock sl(_mutex, other._mutex);
		std::swap(_data, other._data);
	}
};
