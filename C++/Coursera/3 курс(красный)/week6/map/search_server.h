#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <mutex>
using namespace std;

class Database
{ // класс данных для поисковой системы
	map<string, map<size_t, size_t>> letter_words; // словарь релевантности документов для данного слова (word (K) -> docid (V/K) -> hitcount(V))
	size_t id_count;
	void ExploreLine(vector<string> doc_words, size_t doc_id); // добавление слов из документа в базу
public:
	Database();
	void Add(const string& document); // добавить строку-документ в базу
	const map<size_t, size_t>& Lookup(const string& word) const; // словарь релевантности документов для слова
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
