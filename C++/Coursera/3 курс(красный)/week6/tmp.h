#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <mutex>
using namespace std;

class Database
{ // класс данных для поисковой системы
	struct WordData
	{ // структура для хранения слов
		string word;
		size_t index; // индекс в векторе
	};
	set<WordData> words; // множество уникальных слов в базе с индексами

	struct WordHits
	{ // структура для хранения релевантности документов для слова
		mutex wmutex; // контроль доступа к слову
		map<size_t, size_t> docs_hits;
	};
	vector<WordHits> word_docs; // данные по релевантности документов для каждого слова

	mutex id_mutex; // контроль доступа к счетчику
	size_t id_count;
	void ExploreDocument(string document, size_t doc_id);
public:
	Database();
	void Add(string document);
	//const map<size_t, size_t>& Lookup(const string& word) const;
};

class SearchServer
{ // класс поисковой системы
	Database data;
	void UpdateDocumentBaseSigleThread(vector<string> document_input);
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input);
	void ConstructRelevant(const map<size_t, vector<size_t>>& hitcount_count, ostream& search_results_output, size_t amount) const;
	void AddQueriesStream(istream& query_input, ostream& search_results_output) const;
};
