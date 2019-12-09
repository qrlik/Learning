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

void Database::Add(const string& document) // получает документ и обновляет данные
{
	for (string& word : SplitLine(document)) // итерируемся по каждому слову
	{
		DocsHits& word_data = letter_words[move(word)];
		auto [it, is_insert] = word_data.id_index.insert({ id_count, word_data.id_hits.size() }); // вставляем doc_id и предполагаемый index в id_hits
		if (is_insert)
		{ // если такого документа в слове еще не было
			word_data.id_hits.push_back({ id_count, 0 }); // Добавляем пару doc_id , hitcount
		}
		++word_data.id_hits[it->second].second; // увеличиваем hitcount для нового/старого документа
	}
	++id_count; // счетчик документов
}

const vector<pair<size_t, size_t>>& Database::Lookup(const string& word) const
{
	if (auto it = letter_words.find(word); it != letter_words.end())
	{
		return it->second.id_hits;
	}
	else
	{
		static const vector<pair<size_t, size_t>> empty;
		return empty;
	}
}

size_t Database::size() const
{
	return id_count;
}

// реализация SearchServer
SearchServer::SearchServer() : search_count(0) {}

SearchServer::SearchServer(istream& document_input)
{
	UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBaseThread(istream& document_input) // обновление базы в отдельном потоке
{
	Database new_data; // временный объект чтобы не блокировать поиск при обновлении
	for (string current_document; getline(document_input, current_document); )
	{
		if (!current_document.empty()) // игнорируем пустые документы
		{
			new_data.Add(current_document);
		}
	}

	lock_guard search_guard(dmutex); // меняем данные если блокировка не у AddQueriesStreamThread
	//data = move(new_data);
	swap(data, new_data); // обновляем данные (с move почему-то грейдер не принимает)
}

void SearchServer::UpdateDocumentBase(istream& document_input) // обновление базы документов
{
	if (data.size() == 0)
	{ // для пустой базы вызываем синхронно
		UpdateDocumentBaseThread(document_input);
	}
	else
	{
		server_futures.push_back(async(&SearchServer::UpdateDocumentBaseThread, this, ref(document_input)));
	}
}

void SearchServer::AddQueriesStreamThread(istream& query_input, ostream& search_results_output)
{
	vector<pair<size_t, int>> docid_count; // счетчик hitcount для всех документов <hitcount, docid>
	size_t relevant_size; // количество выводимых документов

	for (string current_query; getline(query_input, current_query); ) // получили строку запроса
	{
		vector<string> query_words = SplitLine(current_query); //вектор слов из запроса

		if (search_count.GetAccess().ref_to_value++ == 0) // первый поток блокирует доступ к данным
		{
			dmutex.lock();
		}

		docid_count.assign(data.size(), make_pair(0, 0)); // делаем нулевую маску по размеру базы
		relevant_size = min(static_cast<size_t>(5), docid_count.size()); // до 5 релевантных документов
		for (const auto& word : query_words) // для каждого слова в запросе
		{
			for (auto [docid, hits] : data.Lookup(word)) // забираем информацию из id_hits
			{
				docid_count[docid].first += hits;
				docid_count[docid].second = docid;
			}
		}

		if (--search_count.GetAccess().ref_to_value == 0) // если все потоки закончили поиск, снимаем блокировку данных
		{
			dmutex.unlock();
		}

		auto lambda = [](pair<size_t, int> lhs, pair<size_t, int> rhs) // сортируем по убыванию hitcount и возрастанию doc_id
		{
			lhs.second = -lhs.second;
			rhs.second = -rhs.second;
			return lhs > rhs;
		};

		partial_sort(docid_count.begin(), docid_count.begin() + relevant_size, docid_count.end(), lambda); // получаем релевантные документы

		search_results_output << current_query << ':'; // для пустого поиска выведет только [текст запроса]:
		for (size_t i = 0; i < relevant_size && docid_count[i].first != 0; ++i) // выводим релевантные документы c ненулевой релевантностью
		{
			search_results_output << " {"
				<< "docid: " << docid_count[i].second << ", "
				<< "hitcount: " << docid_count[i].first << '}';
		}
		search_results_output << '\n';
	}
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output)
{ // отправляем поиск в отдельный поток
	server_futures.push_back(async(&SearchServer::AddQueriesStreamThread, this, ref(query_input), ref(search_results_output)));
}
