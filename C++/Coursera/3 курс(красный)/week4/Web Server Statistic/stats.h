#pragma once

#include <string_view>
#include <map>
#include <array>

#include "http_request.h"

struct HttpRequest
{ // запрос к серверу
	std::string_view method; // метод (get, post, put, delete)
	std::string_view uri; // /product | /order | /basket | /help | / (MAIN)
	std::string_view protocol; // HTTP/1.1
};

class StatPiece
{
	map<string_view, int> counts;
	const string_view default_key;
public:
	template <typename Cont> StatPiece(const Cont& known_key, string_view default_value) : default_key(default_value)
	{
		for (const auto& key : known_key)
		{
			counts[key] = 0;
		}
		counts[default_key] = 0;
	}
	void Add(string_view value);
	const map<string_view, int>& GetCounts() const;
};

class Stats
{ // база для изучения запросов к серверу
	inline static const array<string, 4> known_methods = { "DELETE", "GET", "POST", "PUT" };
	inline static const string default_method = "UNKNOWN";

	inline static const array<string, 5> known_uris = { "/", "/basket", "/help", "/order", "/product" };
	inline static const string default_uri = "unknown";

	StatPiece methods;
	StatPiece uris;
public:
	Stats();
	void AddMethod(string_view method); // если метод, переданный в метод AddMethod, не поддерживается нашим сервером (список поддерживаемых методов приведён выше), то нужно на единицу увеличить счётчик для метода «UNKNOWN»
	void AddUri(string_view uri); // если URI, переданный в метод AddUri, не поддерживается нашим сервером, то нужно на единицу увеличить счётчик для URI «unknown»
	const map<string_view, int>& GetMethodStats() const; // метод GetMethodStats возвращает словарь, в котором для каждого метода хранится, сколько раз он встретился в качестве аргумента метода AddMethod
	const map<string_view, int>& GetUriStats() const; // метод GetUriStats работает аналогично для URI
};

HttpRequest ParseRequest(string_view line);
