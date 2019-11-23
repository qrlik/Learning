#include <string>
#include <map>

using namespace std;

class Translator {
public:
	void Add(string_view source, string_view target)
	{
		auto src_it = m_source.find(source.data()); // ищем исходное слово
		if (src_it != m_source.end())
		{ // слово уже есть, нужно поменять перевод
			m_target.erase(src_it->second.data()); // удаляем старый перевод
		}
		else
		{ // слова нет, добавляем переводы
			auto src_insrt = m_source.insert({ source.data(), string_view() }); // вставляем исходник, пока без ссылки
			src_it = src_insrt.first;
		}
		auto [trgt_it, is_insrt] = m_target.insert({ target.data(), src_it->first }); // добавляем новый перевод
		src_it->second = trgt_it->first;  // обновляем ссылку на новый перевод
	}

	string_view TranslateForward(string_view source) const
	{
		return Translate(m_source, source);
	}

	string_view TranslateBackward(string_view target) const
	{
		return Translate(m_target, target);
	}

private:
	string_view Translate(const map<string, string_view>& dict, string_view sv) const
	{
		if (auto it = dict.find(sv.data()); it != dict.end())
		{
			return it->second;
		}
		return string_view();
	}

	map<string, string_view> m_source; // первый язык
	map<string, string_view> m_target; // второй язык
};

void TestSimple() {
	Translator translator;
	translator.Add(string("okno"), string("window"));
	translator.Add(string("stol"), string("table"));

	ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
	ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
	ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSimple);
	return 0;
}
