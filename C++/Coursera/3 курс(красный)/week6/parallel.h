#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <mutex>
using namespace std;

class Database
{ // класс данных для поисковой системы
	struct LetterAccess
	{ // структура для доступа к словарю слов по их первой букве
		mutex letter_mutex; // контроль доступа к букве
		map<string, map<size_t, size_t>> letter_words; // словарь релевантности документов для данного слова (word (K) -> docid (V/K) -> hitcount(V))
	};
	vector<LetterAccess> alphabet;
	mutex id_mutex; // контроль доступа к счетчику документов
	size_t id_count;

	void ExploreLine(vector<string> doc_words, size_t doc_id); // добавление слов из документа в базу
	size_t WordToIndex(const string& word) const; // преобразование слова в индекс для alphabet
public:
	Database();
	const Database& operator=(Database&& other) noexcept;
	void Add(const string& document); // добавить строку-документ в базу
	const map<size_t, size_t>& Lookup(const string& word) const; // словарь релевантности документов для слова
};

class SearchServer
{ // класс поисковой системы
	Database data;
	void ConstructRelevant(const map<size_t, vector<size_t>>& hitcount_count, ostream& search_results_output, size_t amount) const; // получить вывод 5 релевантных документов по словарю релевантности
	void UpdateDocumentBaseSingleThread(Database& new_data, vector<string> document_input);
	ostringstream AddQueriesStreamSingleThread(vector<string> query_input) const; // однопоточная обработка запросов
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input); // обновить базу документов
	void AddQueriesStream(istream& query_input, ostream& search_results_output) const; // обработать запросы
};
