#pragma once

template<typename T>
class MyUniquePTR {
	T* ptr{};
public:
	MyUniquePTR() = default;
	explicit MyUniquePTR(T* other) : ptr(other) {}
	explicit MyUniquePTR(T value) : ptr(new T(std::move(value))) {}
	~MyUniquePTR() {
		delete ptr;
	}

	MyUniquePTR(const MyUniquePTR&) = delete;
	MyUniquePTR(MyUniquePTR&& other) noexcept : ptr(other.ptr) {
		other.ptr = nullptr;
	}

	MyUniquePTR& operator=(const MyUniquePTR&) = delete;
	MyUniquePTR& operator=(MyUniquePTR&& other) {
		if (this != &other) {
			MyUniquePTR(std::move(other)).swap(*this);
		}
		return *this;
	}

	void swap(MyUniquePTR& other) noexcept {
		std::swap(ptr, other.ptr);
	}

	T* get() noexcept {
		return ptr;
	}
	
	T* const get() const noexcept {
		return ptr;
	}
	
	void set(T value) {
		if (!ptr) {
			ptr = new T(std::move(value));
		}
		else {
			*ptr = std::move(value);
		}
	}

	T& operator*() const {
		return *ptr;
	}

	T* operator->() noexcept {
		return ptr;
	}
	
	T* const operator->() const noexcept {
		return ptr;
	}
	
	explicit operator bool() const noexcept {
		return ptr != nullptr;
	}
};
