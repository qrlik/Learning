#pragma once
#include <string>
#include <iostream>
#include <type_traits>
#include <queue>
#include <memory>

// Задание 1.
template<typename Cont>
void PrintAnyCont(const Cont& cont, std::ostream& output = std::cout) {
	for (const auto& element : cont) {
		if constexpr (std::is_pointer_v<std::remove_reference_t<decltype(element)>> &&
			!std::is_same_v<const char*, std::decay_t<decltype(element)>>) {
			output << *element << ' ';
		}
		else {
			output << element << ' ';
		}
	}
}

//Задание 2.
template<typename T1, typename T2>
void Summation(T1& first, const T2& second) {
	if constexpr (std::is_same_v<std::vector<T2>&, decltype(first)>) {
		for (auto& element : first) {
			element += second;
		}
	}
	else {
		first += second;
	}
}

//Задание 3. (stack, queue, priority_queue)
template<typename Adap>
void PrintAnyAdap(Adap& adapter, std::ostream& output = std::cout) {
	using Adap_type = typename Adap::value_type;
	std::vector<Adap_type> data;
	data.reserve(adapter.size());

	if constexpr (std::is_same_v<Adap, std::queue<Adap_type>>) {
		while (!adapter.empty()) {
			output << adapter.front() << ' ';
			data.push_back(std::move(adapter.front()));
			adapter.pop();
		}

		for (auto& element : data) {
			adapter.push(std::move(element));
		}
	}
	else { // stack or priority_queue
		while (!adapter.empty()) {
			output << adapter.top() << ' ';
			data.push_back(std::move(adapter.top()));
			adapter.pop();
		}

		for (auto itb = data.rbegin(), ite = data.rend(); itb != ite; ++itb) {
			adapter.push(std::move(*itb));
		}
	}
}

//Задание 4.
struct SharedCompare {
	template<typename T>
	bool operator()(std::shared_ptr<T> lhs, std::shared_ptr<T> rhs) const {
		if (!lhs) {
			return true;
		}
		else if (!rhs) {
			return false;
		}
		else {
			//return *lhs < *rhs;
			return std::less<T>()(*lhs, *rhs);
		}
	}
};

//Задание 6.
class human {
	/* строить иерархию стоит только в одном направлении, во избежание циклических ссылок.
	либо вводить проверки чтобы не было ситуации когда в _parent лежит владеющий указатель на потомка*/
	std::pair<std::shared_ptr<human>, std::shared_ptr<human>> _parents;
	std::vector<std::weak_ptr<human>> _childs;
	std::string _name;
	size_t _generation;
	bool _alive;

	void print_level(std::ostream& output, size_t level) const;
public:
	static std::shared_ptr<human> child(human&& new_human,
		std::shared_ptr<human> f_parent = {}, std::shared_ptr<human> s_parent = {});

	explicit human(std::string name, bool is_alive) : _name(std::move(name)),
		_alive(is_alive), _generation(0) {}
	human(const human&) = delete;
	human(human&&) = default;
	human& operator=(const human&) = delete;
	human& operator=(human&&) = default;
	void print(std::ostream& output = std::cout) const;
};

//Задание 7.
template<typename T, size_t size>
class MyArray {
	T _ar[size]{}; //как обеспечить инициализацию элементов базового типа по умолчанию нулем?

public:
	constexpr MyArray() = default;
	constexpr MyArray(const T* ptr) {
		for (size_t i = 0; i < size; ++i) {
			_ar[i] = ptr[i];
		}
	}
};

template<class T, size_t size>
MyArray(const T(&)[size]) -> MyArray<T, size>;
