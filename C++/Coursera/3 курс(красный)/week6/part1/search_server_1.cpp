#include "search_server.h"

#include <algorithm>
#include <sstream>
#include <iterator>

vector<string> SplitLine(const string& line)
{
	istringstream words_input(line);
	return { istream_iterator<string>(words_input), istream_iterator<string>() };
}

// реализация Database
Database::Database() :id_count(0) {}

void Database::Add(const string& document) // получает объект и обновляет данные
{
	for (string& word : SplitLine(document))
	{
		DocsHits& word_data = letter_words[move(word)];
		auto [it, is_insert] = word_data.id_index.insert({ id_count, word_data.id_hits.size() }); // вставляем doc_id и предполагаемый index в id_hits
		if (is_insert)
		{ // если такого документа в слове еще не было
			word_data.id_hits.push_back({ id_count, 0 }); // Добавляем пару doc_id , hitcount
		}
		++word_data.id_hits[it->second].second; // увеличиваем hitcount для нового/старого документа
	}
	++id_count;
}

const vector<pair<size_t, size_t>>& Database::Lookup(const string& word) const
{
	return letter_words.at(word).id_hits;
}

size_t Database::size() const
{
	return id_count;
}

// реализация SearchServer
SearchServer::SearchServer(istream& document_input)
{
	UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) // обновление базы документов (максимум 5*10^4 штук)
{
	Database new_data;
	for (string current_document; getline(document_input, current_document); )
	{
		if (!current_document.empty())
		{
			new_data.Add(current_document);
		}
	}
	data = move(new_data);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) const
{
	vector<pair<size_t, int>> docid_count(data.size(), make_pair(0, 0)); // счетчик hitcount для всех документов <hitcount, docid>
	const size_t relevant_size = min(static_cast<size_t>(5), data.size());

	for (string current_query; getline(query_input, current_query); ) // получили запрос
	{
		vector<string> query_words = SplitLine(current_query); //вектор слов из запроса

		for (const auto& word : query_words) // для каждого слова в запросе
		{
			try
			{ // проверяем есть ли слово в базе
				for (auto [docid, hits] : data.Lookup(word)) // забираем информацию из id_hits
				{
					docid_count[docid].first += hits;
					docid_count[docid].second = docid;
				}
			}
			catch (out_of_range&)
			{
				continue;
			}
		}

		auto lambda = [](pair<size_t, int> lhs, pair<size_t, int> rhs)
		{
			lhs.second = -lhs.second;
			rhs.second = -rhs.second;
			return lhs > rhs;
		};

		partial_sort(docid_count.begin(), docid_count.begin() + relevant_size, docid_count.end(), lambda); // получаем первые 5 релевантных документов

		search_results_output << current_query << ':'; // для пустого поиска выведет только [текст запроса]:
		for (size_t i = 0; i < relevant_size && docid_count[i].first != 0; ++i) // выводим до 5 самых релевантных документов c ненулевой релевантностью
		{
			search_results_output << " {"
				<< "docid: " << docid_count[i].second << ", "
				<< "hitcount: " << docid_count[i].first << '}';
		}
		search_results_output << '\n';

		docid_count.assign(data.size(), make_pair(0, 0)); // обнуляем значения в векторе
	}
}
