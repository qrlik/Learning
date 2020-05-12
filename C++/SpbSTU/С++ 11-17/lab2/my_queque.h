#pragma once
#include <initializer_list>
#include <cassert>
#include <tuple>

template<typename T>
class Iterator {
	T* _ptr;
	size_t _index;
	size_t _capacity;
public:
	Iterator(T* other, size_t index, size_t capacity) noexcept :
		_ptr(other), _index(index), _capacity(capacity) {}

	T& operator*() const {
		return _ptr[_index];
	}

	T* operator->() const noexcept {
		return _ptr;
	}

	bool operator!=(const Iterator& other) const noexcept {
		return std::tie(_ptr, _index, _capacity) !=
			std::tie(other._ptr, other._index, other._capacity);
	}

	operator bool() const noexcept {
		return _ptr != nullptr;
	}

	Iterator& operator++() {
		_index = (_index + 1) % _capacity;
		return *this;
	}

	Iterator operator++(int) {
		Iterator tmp(*this);
		_index = (_index + 1) % _capacity;
		return tmp;
	}
};

template<typename T>
class MyQueue {
	inline const static double _factor = 1.5;
	size_t IncByFactor(size_t value) const { /* увеличение в _factor + 1 элемент для end() */
		assert(_factor >= 1.);
		size_t result = static_cast<size_t>(value * _factor);
		return (result == value) ? result + 2 : result + 1;
	}

	T* _ar{};

	size_t _capacity{0}; // ячейка для end() и корректной работы Iterator
	size_t _size{0};
	size_t _first{0};
	size_t _last{0}; // _last = (m_first + m_n) % m_cap

	void addCap() {
		_capacity = IncByFactor(_size);
		T* ar_tmp = new T[_capacity];
		for (size_t i = 0; i < _size; ++i) {
			ar_tmp[i] = std::move(this->operator[](i));
		}
		delete[] _ar;

		_ar = ar_tmp;
		_first = 0;
		_last = _size;
	}

	void addValue(T value) {
		_ar[_last] = std::move(value);
		++_size;
		_last = (_first + _size) % _capacity;
	}

	T& operator[](size_t i) {
		return _ar[(_first + i) % _capacity];
	}

	const T& operator[](size_t i) const {
		return _ar[(_first + i) % _capacity];
	}
public:
	MyQueue() = default;
	explicit MyQueue(std::initializer_list<T> init) : _ar(new T[IncByFactor(init.size())]), _capacity(IncByFactor(init.size())),
		_size(0), _first(0), _last(0) {
		for (auto& element : init) {
			addValue(std::move(element));
		}
	}
	MyQueue(size_t count, const T& value) : _ar(new T[IncByFactor(count)]), _capacity(IncByFactor(count)),
		_size(count), _first(0), _last(count) {
		for (size_t i = 0; i < _size; ++i) {
			_ar[i] = value;
		}
	}
	~MyQueue() { delete[] _ar; }

	MyQueue(const MyQueue& other) : _ar(new T[IncByFactor(other._size)]), _capacity(IncByFactor(other._size)),
		_size(other._size), _first(0), _last(other._size) {
		for (size_t i = 0; i < _size; ++i) {
			_ar[i] = other[i];
		}
	}

	MyQueue(MyQueue&& other) noexcept : _ar(other._ar), _capacity(other._capacity),
		_size(other._size), _first(other._first), _last(other._last) {
		other._ar = nullptr;
		other._capacity = 0;
		other._size = 0;
		other._first = 0;
		other._last = 0;
	}

	MyQueue& operator=(const MyQueue& other) {
		if (this != &other) {
			//MyQueue(other).swap(*this);

			if (capacity() < other._size) {
				delete[] _ar;
				_capacity = IncByFactor(other._size);
				_ar = new T[_capacity];
			}
			_size = other._size;
			_first = 0;
			_last = _size;

			for (size_t i = 0; i < _size; ++i) {
				_ar[i] = other[i];
			}
		}
		return *this;
	}

	MyQueue& operator=(MyQueue&& other) noexcept {
		if (this != &other) {
			MyQueue(std::move(other)).swap(*this);

			//delete[] _ar;
			//_ar = other._ar;
			//_capacity = other._capacity;
			//_size = other._size;
			//_first = other._first;
			//_last = other._last;
			//other._ar = nullptr;
			//other._capacity = 0;
			//other._size = 0;
			//other._first = 0;
			//other._last = 0;
		}
		return *this;
	}

	T& front() {
		return this->operator[](0);
	}

	const T& front() const {
		return this->operator[](0);
	}

	T& back() {
		return this->operator[](_size - 1);
	}

	const T& back() const {
		return this->operator[](_size - 1);
	}

	bool empty() const noexcept {
		return _size == 0;
	}

	size_t size() const noexcept {
		return _size;
	}

	size_t capacity() const noexcept {
		return (_capacity > 0) ? _capacity - 1 : 0;
	}

	template<typename T>
	void push(T&& value) {
		if (_size + 1 >= _capacity) { /* учет ячейки под end() */
			addCap();
		}
		addValue(std::forward<T>(value));
	}

	void pop() noexcept {
		if (!_ar || _size == 0) {
			return;
		}
		--_size;
		_first = (_last + _capacity - _size) % _capacity;
	}

	void swap(MyQueue& other) noexcept {
		std::swap(_ar, other._ar);
		std::swap(_capacity, other._capacity);
		std::swap(_size, other._size);
		std::swap(_first, other._first);
		std::swap(_last, other._last);
	}

	auto begin() noexcept {
		return Iterator(_ar, _first, _capacity);
	}

	auto begin() const noexcept {
		return Iterator<const T>(_ar, _first, _capacity);
	}

	auto end() noexcept {
		return Iterator(_ar, _last, _capacity);
	}

	auto end() const noexcept {
		return Iterator<const T>(_ar, _last, _capacity);
	}
};
