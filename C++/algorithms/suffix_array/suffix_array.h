#pragma once
#include <string_view>
#include <vector>

class Suffix_Array {
	std::vector<std::string_view> suffixes; // half sensitive, half insensitive
public:
	Suffix_Array() = default;
	Suffix_Array(std::string_view str);
	Suffix_Array(const Suffix_Array&) = default;
	Suffix_Array& operator=(const Suffix_Array&) = default;
	Suffix_Array(Suffix_Array&&) = default;
	Suffix_Array& operator=(Suffix_Array&&) = default;

	bool is_contain(std::string_view str, bool is_case_sensitive = false) const;
};
