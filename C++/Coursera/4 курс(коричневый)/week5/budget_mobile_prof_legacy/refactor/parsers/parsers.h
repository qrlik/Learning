#pragma once
#include <optional>
#include <string_view>
#include <stdexcept>
#include <sstream>

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

int ConvertToInt(std::string_view str);
std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view str, std::string_view delimiter = " ");
std::pair<std::string_view, std::string_view> SplitTwo(std::string_view str, std::string_view delimiter = " ");
std::string_view ReadToken(std::string_view& str, std::string_view delimiter = " ");

