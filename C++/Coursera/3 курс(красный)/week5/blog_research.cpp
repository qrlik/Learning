#include <map>
#include <string>
#include <sstream>
#include <future>
#include <algorithm>
#include <functional>
using namespace std;

struct Stats
{
	map<string, int> word_frequences; // ключевые слова и их количество в тексте

	void operator += (const Stats& other) // суммирование словарей статистики ключевых слов
	{
		for (const auto& [key, value] : other.word_frequences)
		{
			word_frequences[key] += value;
		}
	}
};

Stats ExploreLine(const set<string>& key_words, const string& line)
{
	Stats result;

	for (size_t not_space = line.find_first_not_of(' '); not_space != line.npos; )
	{
		size_t space = line.find_first_of(' ', not_space); // находим первый пробел (конец слова)

		string word = line.substr(not_space, space - not_space); // 2 копирование
		if (key_words.find(word) != key_words.end())
		{
			++result.word_frequences[move(word)]; // заносим слово в словарь
		}

		not_space = line.find_first_not_of(' ', space); // ищем начало следующего слова
	}

	return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input)
{
	Stats result;
	for (string line; getline(input, line); ) // для каждой строки в буффере выполняем поиск ключевых слов
	{
		result += ExploreLine(key_words, line);
	}
	return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input)
{
	Stats result;
	vector<future<Stats>> futures;
	vector<stringstream> buffers;

	while (input)
	{
		string line;
		stringstream ss;
		for (size_t i = 0; i < 10000 && getline(input, line); ++i) // 1 копирование в буффер
		{ // Заполняем буффер строками
			ss << line << '\n';
		}
		buffers.push_back(move(ss)); // заполняем вектор буфферов
	}

	for (auto& buffer : buffers)
	{
		futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(buffer))); // создаем потоки для обработки буфферов
	}

	for (auto& f : futures)
	{
		result += f.get(); // суммируем результаты
	}

	return result;
}
