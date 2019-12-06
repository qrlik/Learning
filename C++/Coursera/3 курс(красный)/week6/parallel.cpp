#include "search_server.h"
#include "iterator_range.h"

#include <future>
#include <sstream>
#include <functional>

vector<string> SplitLine(const string& line) // разбить строку на слова
{
	vector<string> result;
	for (size_t not_space = line.find_first_not_of(' '); not_space != line.npos; )
	{
		size_t space = line.find_first_of(' ', not_space); // находим первый пробел (конец слова)
		result.push_back(line.substr(not_space, space - not_space)); // заносим слово в вектор
		not_space = line.find_first_not_of(' ', space); // ищем начало следующего слова
	}
	return result;
}

// реализация Database
Database::Database() :alphabet(25), id_count(0) {} // вектор для распараллеливания доступа по первой букве (алфавит из 25 букв)

const Database& Database::operator=(Database&& other) noexcept
{
	for (size_t i = 0; i < 25; ++i)
	{
		alphabet[i].letter_words = move(other.alphabet[i].letter_words);
	}
	id_count = other.id_count;
	other.id_count = 0;
	return *this;
}

size_t Database::WordToIndex(const string& word) const
{
	return tolower(word[0]) - 'a';
}

void Database::ExploreLine(vector<string> doc_words, size_t doc_id)
{
	for (string& word : doc_words)
	{
		LetterAccess& letter = alphabet[WordToIndex(word)];
		lock_guard letter_guard(letter.letter_mutex); // синхронизируем доступ к словарю по первой букве
		++letter.letter_words[move(word)][doc_id]; // перемещаем слова в словарь,
	}
}

void Database::Add(const string& document) // получает объект и обновляет данные
{
	size_t current_id;
	{
		lock_guard id_guard(id_mutex);  // синхронизируем счетчик документов
		current_id = id_count++;
	}
	ExploreLine(SplitLine(document), current_id); // разбиваем документ и обновляем data для текущего id
}

const map<size_t, size_t>& Database::Lookup(const string& word) const
{
	return alphabet[WordToIndex(word)].letter_words.at(word);
}

// реализация SearchServer
SearchServer::SearchServer(istream& document_input)
{
	UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBaseSingleThread(Database& new_data, vector<string> document_input)
{
	for (const string& current_document : document_input)
	{ // одного копирования в SplitLine не избежать, поэтому дальше можно не move
		new_data.Add(current_document);
	}
}
// ДУБЛИРОВАНИЕ КОДА
void SearchServer::UpdateDocumentBase(istream& document_input) // обновление базы документов (максимум 5*10^4 штук)
{
	Database new_data;
	vector<future<void>> futures;
	const size_t buffer_size = 2500;
	vector<string> buffer;
	buffer.reserve(buffer_size);

	while (document_input)
	{
		string query;
		for (size_t i = 0; i < buffer_size && getline(document_input, query); ++i)
		{
			buffer.push_back(move(query));
		}
		futures.push_back(async(&SearchServer::UpdateDocumentBaseSingleThread, this, ref(new_data), move(buffer))); // отправляем в поток
		buffer.reserve(buffer_size);
	}

	for (auto& f : futures)
	{
		f.get(); // получаем результаты
	}

	data = move(new_data);
}

void SearchServer::ConstructRelevant(const map<size_t, vector<size_t>>& hitcount_count, ostream& search_results_output, size_t amount) const
{
	if (!hitcount_count.empty())
	{ // т.к 0 hitcount быть не может, поэтому проверяем что были ли вообще совпадения в базе
		auto highest_hitcount = hitcount_count.rbegin(); // hitcount - vector<docid>
		for (size_t i = 0; i < amount && highest_hitcount != hitcount_count.rend(); ++highest_hitcount) // выводим до 5 самых релевантных документов
		{
			for (size_t j = 0; i < amount && j < highest_hitcount->second.size(); ++i, ++j)
			{
				search_results_output << " {"
					<< "docid: " << highest_hitcount->second[j] << ", "
					<< "hitcount: " << highest_hitcount->first << '}';
			}
		}
	}
	search_results_output << '\n';
}

ostringstream SearchServer::AddQueriesStreamSingleThread(vector<string> query_input) const
{
	ostringstream search_results_output;
	for (const string& current_query : query_input) // получили запрос
	{
		vector<string> query_words = SplitLine(current_query); //вектор слов из запроса

		map<size_t, size_t> docid_count; // счетчик hitcount
		for (const auto& word : query_words) // для каждого слова в запросе
		{
			try
			{ // проверяем есть ли слово в базе
				for (auto [docid, hitcount] : data.Lookup(word)) // проходим по всем документам где есть это слово
				{
					docid_count[docid] += hitcount; // прибавляем hitcount для каждого
				}
			}
			catch (out_of_range&)
			{
				continue;
			}
		}

		map<size_t, vector<size_t>> hitcount_count; // hitcount - вектор документов (вектор сразу отсортирован по возврастанию)
		for (auto [docid, hitcount] : docid_count) // проходим по всем документам
		{
			hitcount_count[hitcount].push_back(docid); // обновляем hitcount
		}

		search_results_output << current_query << ':'; // для пустого поиска выведет только [текст запроса]:
		ConstructRelevant(hitcount_count, search_results_output, 5); // отправить в результат до N самых релевантных документов
	}
	return search_results_output;
}
// ДУБЛИРОВАНИЕ КОДА
void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) const
{
	vector<future<ostringstream>> futures;
	const size_t buffer_size = 25000;
	vector<string> buffer;
	buffer.reserve(buffer_size);

	while (query_input)
	{
		string query;
		for (size_t i = 0; i < buffer_size && getline(query_input, query); ++i)
		{
			buffer.push_back(move(query));
		}
		futures.push_back(async(&SearchServer::AddQueriesStreamSingleThread, this, move(buffer))); // отправляем в поток
		buffer.reserve(buffer_size);
	}

	for (auto& f : futures)
	{
		search_results_output << f.get().str(); // получаем результаты
	}
}
