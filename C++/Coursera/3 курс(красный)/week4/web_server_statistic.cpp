#pragma once

#include <string_view>
#include <map>
#include <array>

using namespace std;

enum METHODS { DELETE, GET, POST, PUT, UNKNOWN_METHOD, METHOD_COUNT}; // методы
enum URI {MAIN, HELP, BASKET, ORDER, PRODUCT, UNKNOWN_URI, URI_COUNT}; // Uniform Resource Identifier

struct HttpRequest
{ // запрос к серверу
	METHODS method; // метод (get, post, put, delete)
	URI	uri; // /product | /order | /basket | /help | / (MAIN)
	string_view protocol; // HTTP/1.1
};

class Stats
{ // база для изучения запросов к серверу
	array<int, METHODS::METHOD_COUNT> methods; // счетчик методов
	array<int, URI::URI_COUNT> uris; // счетчик идентификаторов
public:
	Stats();
	void AddMethod(METHODS method); // если метод, переданный в метод AddMethod, не поддерживается нашим сервером (список поддерживаемых методов приведён выше), то нужно на единицу увеличить счётчик для метода «UNKNOWN»
	void AddUri(URI uri); // если URI, переданный в метод AddUri, не поддерживается нашим сервером, то нужно на единицу увеличить счётчик для URI «unknown»
	map<string_view, int> GetMethodStats() const; // метод GetMethodStats возвращает словарь, в котором для каждого метода хранится, сколько раз он встретился в качестве аргумента метода AddMethod
	map<string_view, int> GetUriStats() const; // метод GetUriStats работает аналогично для URI
};

HttpRequest ParseRequest(string_view line);

#include "stats.h"

string_view GetWord(string_view& line)
{
	size_t not_space = line.find_first_not_of(' ');
	line.remove_prefix(not_space);
	size_t space = line.find(' '); // нашли пробел
	string_view result = line.substr(0, space); // записали слово
	line.remove_prefix(space + 1); // удалили слово из исходника
	return result;
}

map<string_view, METHODS> Methods_Str
{
	{"DELETE", DELETE},
	{"GET", GET},
	{"POST", POST},
	{"PUT", PUT},
	{"UNKNOWN", UNKNOWN_METHOD}
};

map<string_view, URI> Uris_Str
{
	{"/", MAIN},
	{"/help", HELP},
	{"/basket", BASKET},
	{"/order", ORDER},
	{"/product", PRODUCT},
	{"unknown", UNKNOWN_URI}
};

HttpRequest ParseRequest(string_view line)
{
	HttpRequest result;

	string_view method = GetWord(line);
	result.method = (Methods_Str.count(method) == 0) ? UNKNOWN_METHOD : Methods_Str[method];

	string_view uri = GetWord(line);
	result.uri = (Uris_Str.count(uri) == 0) ? UNKNOWN_URI : Uris_Str[uri];

	result.protocol = GetWord(line);

	return result;
}

Stats::Stats() : methods({ 0 }), uris({ 0 }) {} // инициализируем все нулями

void Stats::AddMethod(METHODS method)
{
	++methods[method];
}

void Stats::AddUri(URI uri)
{
	++uris[uri];
}

map<string_view, int> Stats::GetMethodStats() const
{
	map<string_view, int> result;
	for (auto [sv, enms] : Methods_Str)
	{
		result[sv] = methods[enms];
	}
	return result;
}

map<string_view, int> Stats::GetUriStats() const
{
	map<string_view, int> result;
	for (auto [sv, enms] : Uris_Str)
	{
		result[sv] = uris[enms];
	}
	return result;
}
