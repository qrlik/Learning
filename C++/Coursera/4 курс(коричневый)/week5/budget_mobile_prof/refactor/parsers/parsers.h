#pragma once
#include <optional>
#include <string_view>
#include <string>
#include <stdexcept>
#include <sstream>

template<typename It>
class Range
{
	It begin_;
	It end_;
public:
	Range(It begin, It end) : begin_(begin), end_(end) {}
	It begin() const { return begin_; }
	It end() const { return end_; }
	size_t size() const { return std::distance(begin_, end_); }
};

template<typename Number>
void ValidateBounds(Number number_to_check, Number min_value, Number max_value)
{
	if (number_to_check < min_value || number_to_check > max_value)
	{
		std::ostringstream error;
		error << number_to_check << " is out of [" << min_value << ", " << max_value << "]";
		throw std::out_of_range(error.str());
	}
}

template <typename Number>
Number ReadNumberOnLine(std::istream& stream) {
	Number number;
	stream >> number;
	std::string dummy;
	std::getline(stream, dummy);
	return number;
}

int ConvertToInt(std::string_view str);
std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view str, std::string_view delimiter = " ");
std::pair<std::string_view, std::string_view> SplitTwo(std::string_view str, std::string_view delimiter = " ");
std::string_view ReadToken(std::string_view& str, std::string_view delimiter = " ");




