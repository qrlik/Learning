#include "search_server.h"
#include "iterator_range.h"

#include <future>

// реализация Database
Database::Database() : word_docs(10000), id_count(0) {} // общее число различных слов во всех документах не превосходит 10000

void Database::ExploreDocument(string document, size_t doc_id) // проходимся по документу, заносим слова в множество
{
	for (size_t not_space = document.find_first_not_of(' '); not_space != document.npos; )
	{
		size_t space = document.find_first_of(' ', not_space); // находим первый пробел (конец слова)

		string word = document.substr(not_space, space - not_space); 
		size_t word_index = words.size();
		words.insert({ move(word), word_index }); // добавляем слово в множество и его индекс в векторе

		lock_guard word_guard(word_docs[word_index].wmutex);
		++word_docs[word_index].docs_hits[doc_id];

		not_space = document.find_first_not_of(' ', space); // ищем начало следующего слова
	}
}

void Database::Add(string document) // получает объект и обновляет данные
{
	size_t current_id;
	{
		lock_guard id_guard(id_mutex); // блокируем изменение счетчика документов
		current_id = id_count++;
	}
	ExploreDocument(document, current_id); // разбиваем документ и обновляем data для текущего id
}

//const map<size_t, size_t>& Database::Lookup(const string& word) const
//{
//	return data_words.at(word);
//}

// реализация SearchServer
SearchServer::SearchServer(istream& document_input)
{
	UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBaseSigleThread(vector<string> document_input) // обновление базы документов (максимум 5*10^4 штук)
{
	for (string& current_document : document_input) // получаем документ и перемещаем в Add
	{
		data.Add(move(current_document));
	}
}

void SearchServer::UpdateDocumentBase(istream& document_input) // обновление базы документов (максимум 5*10^4 штук)
{
	vector<future<void>> futures;
	const size_t buffer_size = 6250; // 1/8
	vector<string> buffer; // буффер для отправки в поток
	buffer.reserve(buffer_size);

	while (document_input)
	{
		string line;
		for (size_t i = 0; i < buffer_size && getline(document_input, line); ++i) // заполняем буффер
		{
			buffer.push_back(move(line));
		}
		futures.push_back(async(&SearchServer::UpdateDocumentBaseSigleThread, this, move(buffer))); // отправляем в поток
		buffer.reserve(buffer_size);
	}

	for (auto& f : futures)
	{
		f.get(); // получаем результаты
	}
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

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) const
{
	for (string current_query; getline(query_input, current_query); ) // получили запрос
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
}
