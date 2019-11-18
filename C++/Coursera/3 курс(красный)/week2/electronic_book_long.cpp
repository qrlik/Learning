#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm>

using namespace std;

typedef int user_id;

class ReadingManager
{
	map<user_id, int> users; // список пользователей и прочтенных страниц
	map<int, int> pages; // список страниц и рейтингов

public:
	void Read(int user_id, int page_count) // log(N) + log(P), N - количество пользователей(максимум 10^5), P - количество страниц (максимум 10^3)
	{ // user дочитал книгу до страницы page
		int old_page = users[user_id]; // сохраняем старое количество страниц (или 0 если пользователя не было)			// log(N)
		users[user_id] = page_count; // меняем/добавляем для пользователя количество страниц на данный момент			// log(N)
		++pages[page_count];
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
		auto it = pages.lower_bound(user_pages);
		int less_users = 0;
		for (; it != pages.begin(); ) // O(P)
		{
			--it;
			less_users += it->second;
		}

		return less_users * 1.0 / (users.size() - 1); // количество недочитавших делим на общее количество минус наш пользователь
	}
};

int main() {
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
