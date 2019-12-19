#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_map>
#include <string>
#include <future>
#include <random>
using namespace std;

template <typename K, typename V, typename Hash = std::hash<K>> class ConcurrentMap
{
public:
	using MapType = unordered_map<K, V, Hash>;

private:
	struct Page
	{ // у каждой страницы свой mutex и словарь
		MapType page_map;
		mutable mutex page_mutex;
	};
	vector<Page> data;
	Hash hasher; // хэшер для ключей
	size_t getIndex(const K& key) const
	{
		return hasher(key) % data.size();
	}

public:
	struct WriteAccess : lock_guard<mutex>
	{
		V& ref_to_value;
		WriteAccess(const K& key, Page& page) : lock_guard(page.page_mutex), ref_to_value(page.page_map[key]) {}
	};

	struct ReadAccess : lock_guard<mutex>
	{
		const V& ref_to_value;
		ReadAccess(const K& key, const Page& page) : lock_guard(page.page_mutex), ref_to_value(page.page_map.at(key)) {}
	};

	explicit ConcurrentMap(size_t bucket_count) : data(bucket_count) {} // создаем bucket_count страниц со своим mutex и подсловарем

	WriteAccess operator[](const K& key)
	{ // получаем номер страницы - от 0 до pages - 1 и ссылку на страницу.
		return { key, data[getIndex(key)] };
	}

	ReadAccess At(const K& key) const
	{
		return { key, data[getIndex(key)] };
	}

	bool Has(const K& key) const
	{
		const Page& search_page = data[getIndex(key)];
		lock_guard<mutex> page_guard(search_page.page_mutex);
		return search_page.page_map.count(key) == 1;
	}

	MapType BuildOrdinaryMap() const
	{ // вернуть сложенный из страниц словарь
		MapType result;
		for (auto& [data, mtx] : data)
		{
			lock_guard<mutex> page_guard(mtx); // безопасно получаем доступ к каждой странице
			result.insert(data.begin(), data.end()); // считываем страницу
		}
		return result;
	}
};

void RunConcurrentUpdates(ConcurrentMap<int, int>& cm, size_t thread_count, int key_count) {
	auto kernel = [&cm, key_count](int seed) {
		vector<int> updates(key_count);
		iota(begin(updates), end(updates), -key_count / 2);
		shuffle(begin(updates), end(updates), default_random_engine(seed));

		for (int i = 0; i < 2; ++i) {
			for (auto key : updates) {
				cm[key].ref_to_value++;
			}
		}
	};

	vector<future<void>> futures;
	for (size_t i = 0; i < thread_count; ++i) {
		futures.push_back(async(kernel, i));
	}
}

void TestConcurrentUpdate() {
	const size_t thread_count = 3;
	const size_t key_count = 50000;

	ConcurrentMap<int, int> cm(thread_count);
	RunConcurrentUpdates(cm, thread_count, key_count);

	const auto result = cm.BuildOrdinaryMap();
	ASSERT_EQUAL(result.size(), key_count);
	for (auto& [k, v] : result) {
		AssertEqual(v, 6, "Key = " + to_string(k));
	}
}

void TestReadAndWrite() {
	ConcurrentMap<size_t, string> cm(5);

	auto updater = [&cm] {
		for (size_t i = 0; i < 50000; ++i) {
			cm[i].ref_to_value += 'a';
		}
	};
	auto reader = [&cm] {
		vector<string> result(50000);
		for (size_t i = 0; i < result.size(); ++i) {
			result[i] = cm[i].ref_to_value;
		}
		return result;
	};

	auto u1 = async(updater);
	auto r1 = async(reader);
	auto u2 = async(updater);
	auto r2 = async(reader);

	u1.get();
	u2.get();

	for (auto f : { &r1, &r2 }) {
		auto result = f->get();
		ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
			return s.empty() || s == "a" || s == "aa";
			}));
	}
}

void TestSpeedup() {
	{
		ConcurrentMap<int, int> single_lock(1);

		LOG_DURATION("Single lock");
		RunConcurrentUpdates(single_lock, 4, 50000);
	}
	{
		ConcurrentMap<int, int> many_locks(100);

		LOG_DURATION("100 locks");
		RunConcurrentUpdates(many_locks, 4, 50000);
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestConcurrentUpdate);
	RUN_TEST(tr, TestReadAndWrite);
	RUN_TEST(tr, TestSpeedup);
}
