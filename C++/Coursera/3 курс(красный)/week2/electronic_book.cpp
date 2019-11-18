#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm>

#include "profile.h"
#include "test_runner.h"

using namespace std;

typedef int user_id;

class ReadingManager
{
	map<user_id, int> users; // список пользователей и прочтенных страниц
	map<int, int> pages; // список страниц и количество читающих данную страницу

public:
	void Read(int user_id, int page_count) // log(N) + log(P), N - количество пользователей(максимум 10^5), P - количество страниц (максимум 10^3)
	{ // user дочитал книгу до страницы page
		int old_page = users[user_id]; // сохраняем старое количество страниц (или 0 если пользователя не было)			// log(N)
		users[user_id] = page_count; // меняем/добавляем для пользователя количество страниц на данный момент			
		++pages[page_count];																							// log(P)
		if (old_page)
		{
			--pages[old_page];
		}
	}

	double Cheer(int user_id) const // O(P)
	{ // вывод - доля существующих пользователей (не считая его самого) прочитала меньшую часть книги, чем он
		if (users.count(user_id) == 0) { // если пользователь еще не читает
			return 0;
		}

		if (users.size() == 1) { // если пользователь единственный читатель
			return 1;
		}

		int user_pages = users.at(user_id); // количество страниц пользователя

		//Good job! (Max time used: 1.66/4.00)

		int done_users = -1;
		for (auto it = pages.lower_bound(user_pages); it != pages.end(); ++it) // O(P) - максимум 10^3 страниц
		{
			done_users += it->second; // число пользователей прочитавших больше
		}
		return (users.size() - 1 - done_users) * 1.0 / (users.size() - 1); // количество недочитавших делим на общее количество минус наш пользователь

		// Failed case #6/9: time limit exceeded (Time used: 5.50/4.00)

		//int less_users = 0;
		//for (auto it = pages.lower_bound(user_pages); it != pages.begin(); ) // O(P) - максимум 10^3 страниц
		//{
		//	--it;
		//	less_users += it->second; // число пользователей прочитавших меньше
		//}
		//return less_users * 1.0 / (users.size() - 1); // количество недочитавших делим на общее количество минус наш пользователь
	}
};

void TestReadPage() {
	{// add user
		ReadingManager manager;
		ASSERT_EQUAL(manager.Cheer(1), 0.0);
		manager.Read(1, 1);
		ASSERT_EQUAL(manager.Cheer(1), 1.0);
	}
	{// add user but cheer not existed
		ReadingManager manager;
		ASSERT_EQUAL(manager.Cheer(1), 0.0);
		manager.Read(1, 1);
		ASSERT_EQUAL(manager.Cheer(2), 0.0);
	}
	{// read twice
		ReadingManager manager;
		manager.Read(1, 1);
		ASSERT_EQUAL(manager.Cheer(1), 1.0);
		manager.Read(1, 2);
		ASSERT_EQUAL(manager.Cheer(1), 1.0);
	}
	{// better than third
		ReadingManager manager;
		manager.Read(1, 1);
		manager.Read(2, 2);
		manager.Read(3, 3);
		ASSERT_EQUAL(manager.Cheer(2), 0.5);
		manager.Read(4, 4);
		ASSERT_EQUAL(manager.Cheer(2), 1.0 / 3.0);
	}
	{// better than 2 third
		ReadingManager manager;
		manager.Read(1, 1);
		manager.Read(2, 2);
		manager.Read(3, 3);
		ASSERT_EQUAL(manager.Cheer(2), 0.5);
		manager.Read(4, 4);
		ASSERT_EQUAL(manager.Cheer(3), 2.0 / 3.0);
	}
	{// read zero
		ReadingManager manager;
		manager.Read(1, 0);
		manager.Read(2, 0);
		manager.Read(3, 1);
		manager.Read(4, 2);
		ASSERT_EQUAL(manager.Cheer(3), 2.0 / 3.0);
	}
	{// all read the same
		ReadingManager manager;
		manager.Read(1, 5);
		manager.Read(2, 5);
		manager.Read(3, 5);
		manager.Read(4, 5);
		ASSERT_EQUAL(manager.Cheer(3), 0.0);
	}
	{// reverse order
		ReadingManager manager;
		manager.Read(4, 5);
		manager.Read(3, 4);
		manager.Read(2, 3);
		manager.Read(1, 2);
		ASSERT_EQUAL(manager.Cheer(3), 2.0 / 3.0);
	}
	{// two users read two times
		ReadingManager manager;
		manager.Read(4, 5);
		manager.Read(3, 4);
		manager.Read(4, 6);
		manager.Read(3, 5);
		ASSERT_EQUAL(manager.Cheer(4), 1.0);
		ASSERT_EQUAL(manager.Cheer(3), 0.0);
		manager.Read(10, 2);
		ASSERT_EQUAL(manager.Cheer(3), 0.5);
		ASSERT_EQUAL(manager.Cheer(4), 1.0);
	}
	{// only one user
		ReadingManager manager;
		manager.Read(4, 5);
		ASSERT_EQUAL(manager.Cheer(4), 1.0);
		manager.Read(4, 6);
		ASSERT_EQUAL(manager.Cheer(4), 1.0);
		manager.Read(4, 100);
		ASSERT_EQUAL(manager.Cheer(4), 1.0);
		ASSERT_EQUAL(manager.Cheer(3), 0.0);
		ASSERT_EQUAL(manager.Cheer(5), 0.0);
	}
	{// ten users
		ReadingManager manager;
		manager.Read(1, 1);
		manager.Read(2, 2);
		manager.Read(3, 3);
		manager.Read(4, 4);
		manager.Read(5, 5);
		manager.Read(6, 6);
		manager.Read(7, 7);
		manager.Read(8, 8);
		manager.Read(9, 9);
		manager.Read(100, 10);
		manager.Read(111, 11);

		ASSERT_EQUAL(manager.Cheer(1), 0.0);
		ASSERT_EQUAL(manager.Cheer(2), 0.1);
		ASSERT_EQUAL(manager.Cheer(3), 0.2);
		ASSERT_EQUAL(manager.Cheer(4), 0.3);
		ASSERT_EQUAL(manager.Cheer(5), 0.4);
		ASSERT_EQUAL(manager.Cheer(6), 0.5);
		ASSERT_EQUAL(manager.Cheer(7), 0.6);
		ASSERT_EQUAL(manager.Cheer(8), 0.7);
		ASSERT_EQUAL(manager.Cheer(9), 0.8);
		ASSERT_EQUAL(manager.Cheer(100), 0.9);
		ASSERT_EQUAL(manager.Cheer(111), 1.0);
		ASSERT_EQUAL(manager.Cheer(12), 0.0);
	}
	{// cheer twice
		ReadingManager manager;

		ASSERT_EQUAL(manager.Cheer(1), 0.0);
		ASSERT_EQUAL(manager.Cheer(1), 0.0);
		manager.Read(1, 1);
		ASSERT_EQUAL(manager.Cheer(1), 1.0);
		ASSERT_EQUAL(manager.Cheer(1), 1.0);

		ASSERT_EQUAL(manager.Cheer(2), 0);
	}
	{// max ID
		const int MAX_USER_COUNT = 100'000;
		ReadingManager manager;
		ASSERT_EQUAL(manager.Cheer(MAX_USER_COUNT), 0.0);
		manager.Read(MAX_USER_COUNT, 1);
		ASSERT_EQUAL(manager.Cheer(MAX_USER_COUNT), 1.0);
	}
	{// max page
		ReadingManager manager;
		ASSERT_EQUAL(manager.Cheer(1), 0.0);
		manager.Read(1, 1000);
		ASSERT_EQUAL(manager.Cheer(1), 1.0);
	}
	{// max ID and max page
		const int MAX_USER_COUNT = 100'000;
		ReadingManager manager;
		ASSERT_EQUAL(manager.Cheer(MAX_USER_COUNT), 0.0);
		manager.Read(MAX_USER_COUNT, 1000);
		ASSERT_EQUAL(manager.Cheer(MAX_USER_COUNT), 1.0);
	}
	{// zero ID
		ReadingManager manager;
		ASSERT_EQUAL(manager.Cheer(0), 0.0);
		manager.Read(0, 1000);
		ASSERT_EQUAL(manager.Cheer(0), 1.0);
	}
	{// from coursera (https://www.coursera.org/learn/c-plus-plus-red/programming/e72ST/eliektronnaia-knigha/discussions/threads/hLK3cT0AEemHtRKqiMW_lA)
		ReadingManager manager;
		manager.Read(1, 1);
		manager.Read(1, 3);
		manager.Read(2, 2);
		ASSERT_EQUAL(manager.Cheer(1), 1.0);
		ASSERT_EQUAL(manager.Cheer(2), 0.0);
	}
}

int main() {
	{
		TestRunner tr;
		RUN_TEST(tr, TestReadPage);
	}

	// Для ускорения чтения данных отключается синхронизация
	// cin и cout с stdio,
	// а также выполняется отвязка cin от cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReadingManager manager;

	int query_count; // максимум 10^6 запросов
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) { // Q * P - 10^6 * 10^3
		string query_type;
		cin >> query_type;
		int user_id;
		cin >> user_id;

		if (query_type == "READ") {
			int page_count;
			cin >> page_count;
			manager.Read(user_id, page_count); // log(N)
		}
		else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n"; // O(P)
		}
	}
	return 0;
}
