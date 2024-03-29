#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class Database
{ // класс данных для поисковой системы
	struct DocsHits
	{ // структура для быстрого итерирования в AddQueriesStream
		vector<pair<size_t, size_t>> id_hits; // вектор документов для слова (id - hitcount)
		map<size_t, size_t> id_index; // словарь для поиска документов (id - индекс в векторе)
	};
	map<string, DocsHits> letter_words; // словарь релевантности документов для данного слова (word (K) -> docid (V/K) -> hitcount(V))
	size_t id_count;
public:
	Database();
	void Add(const string& document); // добавить строку-документ в базу
	const vector<pair<size_t, size_t>>& Lookup(const string& word) const; // словарь релевантности документов для слова
	size_t size() const;
};

class SearchServer
{ // класс поисковой системы
	Database data;
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input); // обновить базу документов
	void AddQueriesStream(istream& query_input, ostream& search_results_output) const; // обработать запросы
};
