#include "stats.h"

string_view GetWord(string_view& line)
{ // получить первое слово
	line.remove_prefix(line.find_first_not_of(' ')); // удалили все пробелы в начале

	size_t space = line.find(' '); // нашли пробел
	string_view result = line.substr(0, space); // записали слово
	line.remove_prefix(space + 1); // удалили слово из исходника

	return result;
}

HttpRequest ParseRequest(string_view line)
{
	HttpRequest result;

	result.method = GetWord(line);
	result.uri = GetWord(line);
	result.protocol = GetWord(line);

	return result;
}

// StatPiece реализация

void StatPiece::Add(string_view value)
{
	if (auto it = counts.find(value); it != counts.end())
	{
		++it->second;
	}
	else
	{
		++counts[default_key];
	}
}

const map<string_view, int>& StatPiece::GetCounts() const
{
	return counts;
}

// Stat реализация

Stats::Stats() : methods(known_methods, default_method), uris(known_uris, default_uri) {}

void Stats::AddMethod(string_view method)
{
	methods.Add(method);
}

void Stats::AddUri(string_view uri)
{
	uris.Add(uri);
}

const map<string_view, int>& Stats::GetMethodStats() const
{
	return methods.GetCounts();
}

const map<string_view, int>& Stats::GetUriStats() const
{
	return uris.GetCounts();
}
