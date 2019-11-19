#include <iomanip>
#include <iostream>
#include <queue>
#include <set>
#include <map>

#include "profile.h"
#include "test_runner.h"

using namespace std;

struct reserve
{ // запись о бронировании
	int64_t time; // время бронирования
	int client_id; // номер клиента
	int rooms; // количество комнат
};

class Hotel
{
	queue<reserve> reserves; // все бронирования за последние сутки
	map<int, int> clients_24; // список клиентов и количество их бронирований
	int64_t rooms_24; // количество забронированных комнат за последние сутки

	void adjust(int64_t time)	// O(logQ) - максимум Q клиентов
	{ // удаление всех старых записей
		while (!reserves.empty() && time - 86400 >= reserves.front().time) // пока очередь не пуста удаляем все записи старше суток
		{
			const reserve& front_rsrv = reserves.front();
			rooms_24 -= front_rsrv.rooms; // вычитаем комнаты из счетчика
			const auto last_client_it = clients_24.find(front_rsrv.client_id); // ищем нужного клиента
			if (--last_client_it->second == 0) // уменьшаем счетчик бронирований
			{ // если бронирований за последние сутки больше нет - удаляем
				clients_24.erase(last_client_it);
			}
			reserves.pop();
		}
	}

public:
	explicit Hotel() : rooms_24(0) {}

	void make_reserve(const reserve& new_rsrv) // сделать запись о бронировании
	{
		rooms_24 += new_rsrv.rooms; // добавляем комнаты в счетчик
		++clients_24[new_rsrv.client_id]; // добавляем клиента в множество
		reserves.push(new_rsrv); // добавляем новую запись в очередь
	}

	size_t day_clients(uint64_t time) // список клиентов за последние сутки
	{
		adjust(time); // удаление записей которым больше суток
		return clients_24.size();
	}

	int64_t day_rooms(uint64_t time) // количество забронированных комнат за последние сутки
	{
		adjust(time); // удаление записей которым больше суток
		return rooms_24;
	}
};

class HotelSystem
{
	map<string, Hotel> hotels; // карта отелей по названию и их данные
	uint64_t last_book_time; // последнее время бронирования
public:
	HotelSystem() : last_book_time(0) {}

	void book(const string& h_name, const reserve& new_rsrv) // L * log(Q) + log(Q)
	{
		last_book_time = new_rsrv.time; // обновляем последнее время брони
		hotels[h_name].make_reserve(new_rsrv); // обновляем данные для отеля
	}

	size_t clients(const string& h_name) 
	{
		return hotels[h_name].day_clients(last_book_time); // количество клиентов за сутки от последней брони
	}

	int64_t rooms(const string& h_name)
	{
		return hotels[h_name].day_rooms(last_book_time);  // количество клиентов за сутки от последней брони
	}
};

int main() {
	// Для ускорения чтения данных отключается синхронизация
	// cin и cout с stdio,
	// а также выполняется отвязка cin от cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	HotelSystem hotels;

	int query_count; // максимум 10^5 запросов
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) // O(Q * L * log(Q)) - Q количество запросов, L - длина строки
	{
		string command_type;
		cin >> command_type;
		string hotel_name;

		if (command_type == "BOOK")
		{ // забронировать клиентом client_id room_count номеров в отеле hotel_name в момент времени time
			reserve new_rsrv;
			cin >> new_rsrv.time >> hotel_name >> new_rsrv.client_id >> new_rsrv.rooms;
			hotels.book(hotel_name, new_rsrv);
		}
		else if (command_type == "CLIENTS")
		{ // вывести количество различных клиентов, бронировавших номера в отеле hotel_name за последние сутки
			cin >> hotel_name;
			cout << hotels.clients(hotel_name) << '\n';
		}
		else if (command_type == "ROOMS")
		{ // вывести количество номеров, забронированных за последние сутки в отеле hotel_name
			cin >> hotel_name;
			cout << hotels.rooms(hotel_name) << '\n';
		}
		else
		{
			throw invalid_argument("Unknown command");
		}
	}
	return 0;
}

/* INPUTE _ OUTPUTE
11
CLIENTS Marriott
ROOMS Marriott
BOOK 10 FourSeasons 1 2
BOOK 10 Marriott 1 1
BOOK 86409 FourSeasons 2 1
CLIENTS FourSeasons
ROOMS FourSeasons
CLIENTS Marriott
BOOK 86410 Marriott 2 10
ROOMS FourSeasons
ROOMS Marriott

0
0
2
3
1
1
10
*/
