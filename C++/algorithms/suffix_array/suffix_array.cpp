#include "suffix_array.h"
#include <algorithm>
#include <execution>
#include <cctype>

using namespace std;

namespace {
	bool LessInsensitive(string_view first, string_view second) {
		auto f_size = first.size(), s_size = second.size();
		for (decltype(f_size) i = 0, size = min(f_size, s_size); i < size; ++i) {
			if (tolower(static_cast<unsigned char>(first[i])) < tolower(static_cast<unsigned char>(second[i])))
				return true;
			if (tolower(static_cast<unsigned char>(second[i])) < tolower(static_cast<unsigned char>(first[i])))
				return false;
		}
		return f_size < s_size;
	}

	bool EqualInsensitive(string_view first, string_view second) {
		if (auto size = first.size(); size == second.size()) {
			for (decltype(size) i = 0; i < size; ++i) {
				if (tolower(static_cast<unsigned char>(first[i])) != tolower(static_cast<unsigned char>(second[i])))
					return false;
			}
			return true;
		}
		return false;
	}

	template<typename It, typename LessCompare, typename EqualCompare>
	bool is_contain_temp(It begin, It end, string_view str,
		LessCompare less_comp, EqualCompare equal_comp) {
		auto result = lower_bound(begin, end, str, less_comp);
		return (result != end && equal_comp(result->substr(0, str.size()),str))
			? true : false;
	}
}

Suffix_Array::Suffix_Array(string_view str) {
	auto half_size = str.size();
	suffixes.resize(half_size * 2);
	for (decltype(half_size) i = 0; i < half_size; ++i) {
		suffixes[i] = str.substr(i);
		suffixes[i + half_size] = suffixes[i];
	}
	auto half_it = next(suffixes.begin(), half_size);
	sort(execution::par_unseq, suffixes.begin(), half_it);
	sort(execution::par_unseq, half_it, suffixes.end(), LessInsensitive);
}

bool Suffix_Array::is_contain(string_view str, bool is_case_sensitive) const {
	auto half_it = next(suffixes.begin(), suffixes.size() / 2);
	if (is_case_sensitive) {
		return is_contain_temp(suffixes.begin(), half_it, str, less<>{}, equal_to<>{});
	}
	return is_contain_temp(half_it, suffixes.end(), str, LessInsensitive, EqualInsensitive);
}
