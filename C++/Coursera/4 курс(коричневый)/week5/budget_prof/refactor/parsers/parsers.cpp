#include "parsers.h"
#include <string>

using namespace std;

int ConvertToInt(string_view str)
{
	size_t pos;
	const int result = stoi(string(str), &pos);
	if (pos != str.length())
	{
		ostringstream error;
		error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
		throw invalid_argument(error.str());
	}
	return result;
}

pair<string_view, optional<string_view>> SplitTwoStrict(string_view str, string_view delimiter)
{
	const size_t pos = str.find(delimiter);
	if (pos == str.npos)
	{
		return { str, nullopt };
	}
	else
	{
		return { str.substr(0, pos), str.substr(pos + delimiter.length()) };
	}
}

pair<string_view, string_view> SplitTwo(string_view str, string_view delimiter)
{
	const auto [lhs, rhs_opt] = SplitTwoStrict(str, delimiter);
	return { lhs, rhs_opt.value_or("") };
}

string_view ReadToken(string_view& str, string_view delimiter)
{
	const auto [lhs, rhs] = SplitTwo(str, delimiter);
	str = rhs;
	return lhs;
}

