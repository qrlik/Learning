#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <iterator>
#include <type_traits>

template<typename Cont>
void PrintAnyCont(const Cont& cont, std::ostream& os = std::cout) {
	std::copy(std::cbegin(cont), std::cend(cont),
		std::ostream_iterator<std::decay_t<decltype(*std::cbegin(cont))>>(os, " ")); // T
}

template<typename Digit>
void Negate(Digit& digit) {
	digit = -digit;
}
template<>
void Negate<std::string>(std::string& str);

template<typename Cont>
void NegateAll(Cont& cont) {
	std::for_each(std::begin(cont), std::end(cont), 
		[](auto& value) { Negate(value); });
}

template<typename Cont>
void IncAnyCont(Cont& cont) {
	std::transform(std::begin(cont), std::end(cont), std::begin(cont),
		[](auto value) { return ++value; });
}

template<typename Cont>
void absSort(Cont& cont) {
	std::sort(std::begin(cont), std::end(cont), 
		[](auto lhs, auto rhs) {
			return std::abs(lhs) < std::abs(rhs); });
}

template<typename First, typename Second>
auto SumCont(const First& f_cont, const Second& s_cont) {
	
	auto [small_size, big_size] = std::minmax(std::size(f_cont), std::size(s_cont));
	auto it_f = std::begin(f_cont);
	auto it_s = std::begin(s_cont);

	std::vector<decltype(*it_f + *it_s)> result;
	result.reserve(big_size);

	auto bin_funct = [](const auto& lhs, const auto& rhs) { return lhs + rhs; };
	for (size_t i = 0; i < small_size; ++i) {
		result.emplace_back(bin_funct(*it_f++, *it_s++));
	}

	copy(it_f, std::end(f_cont), std::back_inserter(result));
	copy(it_s, std::end(s_cont), std::back_inserter(result));
	
	return result;
}

template<typename Source, typename Left_cont, typename Right_cont, typename Func>
void Separate(const Source& source, Left_cont& lhs_c, Right_cont& rhs_c, Func predicate) {
	for (const auto& element : source) {
		if (predicate(element)) {
			lhs_c.insert(lhs_c.end(), element);
		}
		else {
			rhs_c.insert(rhs_c.end(), element);
		}
	}
}

enum class COLORS { BLUE = 0, RED = 0, GREEN };

template<typename ENUM>
class EnumMap {
	static const std::unordered_map<std::string, ENUM> str_to_enum;
public:
	static const auto& getEnumToStrMap() {
		return str_to_enum;
	}
};

template<typename ENUM>
std::string enumToString(ENUM value) {
	const auto& str_to_enum = EnumMap<ENUM>::getEnumToStrMap();

	for (const auto& [key, enum_value] : str_to_enum) {
		if (value == enum_value) {
			return key;
		}
	}
	throw std::invalid_argument("No such string-key for ENUM");
}

template<typename ENUM>
ENUM stringToEnum(const std::string& str) {
	const auto& str_to_enum = EnumMap<ENUM>::getEnumToStrMap();

	return str_to_enum.at(str);
}
