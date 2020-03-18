#pragma once
#include <vector>
#include <optional>
#include <type_traits>
#include <algorithm>

template<typename T>
class UniqueVector {
	class Bounds {
		T _lower;
		T _upper;
	public:
		explicit Bounds(std::pair<T,T> p) : _lower(std::min(p.first, p.second)),
			_upper(std::max(p.first, p.second)) {}
		template<typename U>
		bool in_range(const U& value) {
			return value >= _lower && value <= _upper;
		};
	};
	std::optional<Bounds> _bounds;
	std::vector<T> _uniques;

	template<typename Cont>
	std::vector<T> ComputeUniques(Cont&& cont) {
		std::vector<T> uniques;
		bool is_rvalue = std::is_rvalue_reference_v<Cont&&>; 

		for (auto& element : cont) {
			if ((!_bounds || _bounds.value().in_range(element)) &&
				std::find(uniques.begin(), uniques.end(), element) == uniques.end()) {
				if (is_rvalue) {
					uniques.emplace_back(std::move(element));
				}
				else {
					uniques.emplace_back(element);
				}
			}
		}

		return uniques;
	}

public:
	template<typename Cont>
	explicit UniqueVector(Cont&& cont, std::optional<std::pair<T, T>> bounds = {}) :
		_bounds(std::move(bounds)), _uniques(ComputeUniques(std::forward<Cont>(cont))) {}

	template<typename Cont>
	void Add(Cont&& cont) {
		auto add_uniques = ComputeUniques(std::forward<Cont>(cont));

		for (auto& element : add_uniques) {
			if (std::find(_uniques.begin(), _uniques.end(), element) == _uniques.end()) {
				_uniques.emplace_back(std::move(element));
			}
		}
	}

	template<typename Cont>
	void Delete(Cont&& cont) {
		auto del_uniques = ComputeUniques(std::forward<Cont>(cont));
		auto delete_it = std::remove_if(_uniques.begin(), _uniques.end(), [&del_uniques](const auto& value) {
			return std::find(del_uniques.begin(), del_uniques.end(), value) != del_uniques.end();
			});
		_uniques.erase(delete_it, _uniques.end());
	}

	template<typename Pred>
	void Sort(Pred predicate) {
		std::sort(_uniques.begin(), _uniques.end(), predicate);
	}

	const auto& getUniques() const {
		return _uniques;
	}
};

