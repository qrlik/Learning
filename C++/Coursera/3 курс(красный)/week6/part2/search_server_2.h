#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <future>

#include "synchronized.h"

using namespace std;

class Database
{ // класс данных для поисковой системы
	struct DocsHits
	{ // структура для быстрого итерирования в AddQueriesStream
		vector<pair<size_t, size_t>> id_hits; // вектор документов для слова (id - hitcount)
		map<size_t, size_t> id_index; // словарь для поиска документов (id - индекс в векторе)
	};
	map<string, DocsHits> letter_words; // словарь релевантности документов для слов
	size_t id_count; // количество документов в базе
public:
	Database();
	void Add(const string& document); // добавить строку-документ в базу
	const vector<pair<size_t, size_t>>& Lookup(const string& word) const; // словарь релевантности документов для слова
	size_t size() const; // количество документов в базе
};

class SearchServer
{ // класс поисковой системы
	mutex dmutex;
	Database data; // Database о оболочке с синхронным доступом

	Synchronized<size_t> search_count; // счетчик потоков в поиске

	vector<future<void>> server_futures;

	void UpdateDocumentBaseThread(istream& document_input);
	void AddQueriesStreamThread(istream& query_input, ostream& search_results_output);
public:
	SearchServer();
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input); // обновить базу документов
	void AddQueriesStream(istream& query_input, ostream& search_results_output); // обработать запросы
};
