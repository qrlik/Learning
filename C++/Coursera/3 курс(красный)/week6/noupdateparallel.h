#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <mutex>
using namespace std;

class Database
{ // класс данных для поисковой системы
	map<string, map<size_t, size_t>> data_words; // слово - документы и их hitcount для данного слова (word (K) -> docid (V/K) -> hitcount(V))
	size_t id_count = 0;

	void ExploreLine(vector<string> doc_words, size_t doc_id);
public:
	void Add(const string& document);
	const map<size_t, size_t>& Lookup(const string& word) const;
};

class SearchServer
{ // класс поисковой системы
	Database data;
	ostringstream AddQueriesStreamSingleThread(vector<string> buffer) const;
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input);
	void ConstructRelevant(const map<size_t, vector<size_t>>& hitcount_count, ostream& search_results_output, size_t amount) const;
	void AddQueriesStream(istream& query_input, ostream& search_results_output) const;
};
