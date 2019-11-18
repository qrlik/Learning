#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm>

using namespace std;

typedef int user_id;

struct Rating
{ // рейтинг страницы
	int current_usr; // количество пользователей на данной странице
	int prev_usr; // количество пользователей, недочитавших до страницы
};

class ReadingManager
{
	map<user_id, int> users; // список пользователей и прочтенных страниц
	map<int, Rating> pages; // список страниц и рейтингов

public:
	void Read(int user_id, int page_count) // log(N) + log(P), N - количество пользователей(максимум 10^5), P - количество страниц (максимум 10^3)
	{ // user дочитал книгу до страницы page
		int old_page = users[user_id]; // сохраняем старое количество страниц (или 0 если пользователя не было)			// log(N)
		users[user_id] = page_count; // меняем/добавляем для пользователя количество страниц на данный момент			// log(N)

		if (pages.count(page_count) == 0)
		{ // если страницы еще не было, нужно ее правильно добавить
			auto it = pages.lower_bound(page_count); // ищем подходящий ключ
			int count_less = (it == pages.begin()) ? 0 : prev(it)->second.current_usr + prev(it)->second.prev_usr; // если вставка будет в начало, количество недочитавших 0
			pages[page_count] = { 1, count_less }; // на новой странице 1 читатель, и count_less недочитавших
		}
		else
		{ // или если страница была, добавляем читателя для нее
			++pages[page_count].current_usr;
		}

		if (old_page)
		{ // если пользователь уже существовал нужно убрать его из числа недочитавших для страниц выше старой, но не выше данной
			for (auto it = pages.upper_bound(old_page); it != pages.upper_bound(page_count); ++it)
			{ //
				--it->second.prev_usr;
			}
			--pages[old_page].current_usr; // а также убрать из читающих у старой книги
		}
		else
		{ // иначе мы просто добавляем недочитавшего для всех страниц выше данной
			for (auto it = pages.upper_bound(page_count); it != pages.end(); ++it)
			{ //
				++it->second.prev_usr;
			}
		}
	}

	double Cheer(int user_id) const // O(1)
	{ // вывод - доля существующих пользователей (не считая его самого) прочитала меньшую часть книги, чем он
		if (users.count(user_id) == 0) { // если пользователь еще не читает
			return 0;
		}

		if (users.size() == 1) { // если пользователь единственный читатель
			return 1;
		}

		int user_pages = users.at(user_id); // количество страниц пользователя

		return pages.at(user_pages).prev_usr * 1.0 / (users.size() - 1); // количество недочитавших делим на общее количество минус наш пользователь
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
			cout << setprecision(6) << manager.Cheer(user_id) << "\n"; // O(1)
		}
	}
	return 0;
}
