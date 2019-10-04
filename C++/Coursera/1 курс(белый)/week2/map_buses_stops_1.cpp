#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

/*
NEW_BUS bus stop_count stop1 stop2 ... — добавить маршрут автобуса с названием bus и stop_count остановками с названиями stop1, stop2, ...
Для каждого запроса NEW_BUS bus stop_count stop1 stop2 ... гарантируется, что маршрут bus отсутствует, количество остановок больше 0,
а после числа stop_count следует именно такое количество названий остановок, причём все названия в каждом списке различны.

BUSES_FOR_STOP stop — вывести названия всех маршрутов автобуса, проходящих через остановку stop.
На запрос BUSES_FOR_STOP stop выведите через пробел список автобусов, проезжающих через эту остановку, в том порядке,
в котором они создавались командами NEW_BUS. Если остановка stop не существует, выведите No stop.

STOPS_FOR_BUS bus — вывести названия всех остановок маршрута bus со списком автобусов, на которые можно пересесть на каждой из остановок.
На запрос STOPS_FOR_BUS bus выведите описания остановок маршрута bus в отдельных строках в том порядке, в котором они были заданы в соответствующей команде NEW_BUS.
Описание каждой остановки stop должно иметь вид Stop stop: bus1 bus2 ..., где bus1 bus2 ... — список автобусов, проезжающих через остановку stop, в порядке,
в котором они создавались командами NEW_BUS, за исключением исходного маршрута bus. Если через остановку stop не проезжает ни один автобус, кроме bus, вместо
списка автобусов для неё выведите no interchange. Если маршрут bus не существует, выведите No bus.

ALL_BUSES — вывести список всех маршрутов с остановками.
На запрос ALL_BUSES выведите описания всех автобусов в алфавитном порядке. Описание каждого маршрута bus должно иметь вид Bus bus: stop1 stop2 ...,
где stop1 stop2 ... — список остановок автобуса bus в порядке, в котором они были заданы в соответствующей команде NEW_BUS. Если автобусы отсутствуют, выведите No buses.

Для каждого запроса NEW_BUS гарантируется, что маршрут bus отсутствует!
Т.к требуется выводить в порядке добавления, а map и set сортирует значения
-> использование в ключе индекса добавления убивает оптимизацию контейнера -> поэтому проще взять два вектора (что хуже для поиска)
Вообще не ясно зачем выводить в порядке добавления маршруты, это глупо. map и set подходят лучше и выводят в алфовитном порядке. Ну раз это надо...
1)vector < pair<string, vector<string>> > - вектор пар (маршрут и вектор остановок)
экономия памяти, имеем нужный порядок внесения данных. проигрыш в скорости
2)map<string, vector<string>> - две карты, одна маршруты и остановки(в порядке добавления), вторая наоборот
занимает больше памяти, сложнее в плане понимания(для меня), но быстрее работает (быстрее поиск в STOPS_FOR_BUS)
подсмотрел на форуме, т.к раз раздел курса по map, но и использоваться он как-то может
1 вариант мне нравится больше, но т.к раздел по картам, делаем конечно второй (а вообще второй вариант оптимизирует именно функцию STOPS_FOR_BUS)
 */

void new_bus(map<string, vector<string>>& bus_stops, map<string, vector<string>>& stop_buses, const string& bus, const string& stop)
{// добавление данных в обе карты
	bus_stops[bus].emplace_back(stop); // гарантируется, что маршрут bus отсутствует
	stop_buses[stop].emplace_back(bus); // из задание - все названия в каждом списке различны! -> одинаковых данных не будет
}

void buses_for_stop(map<string, vector<string>>& stop_buses, const string& stop) // лучше const map и at()
{// вывести названия всех маршрутов через остановку
	if (stop_buses.count(stop) != 0) // если данная остановка есть
	{
		for (const auto& bus : stop_buses[stop]) // выводим все маршруты
		{
			cout << bus << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "No stop" << endl;
	}
}

void stop_for_bus(map<string, vector<string>>& bus_stops, map<string, vector<string>>& stop_buses, const string& bus) // лучше const map и at()
{ // вывести названия всех остановок маршрута bus со списком автобусов, на которые можно пересесть на каждой из остановок
	if (bus_stops.count(bus) != 0) // если данный маршрут существует
	{
		for (const auto& stop : bus_stops[bus]) // получаем все остановки по данному маршруту
		{
			cout << "Stop " << stop << ": ";
			if (stop_buses[stop].size() > 1) // если на остановке есть другие автобусы, кроме нашего
			{
				for (const auto& it_bus : stop_buses[stop]) // получаем все автобусы по данной остановке
				{
					if (it_bus == bus)
					{
						continue;
					}
					cout << it_bus << " ";
				}
				cout << endl;
			}
			else
			{
				cout << "no interchange" << endl;
			}
		}
	}
	else
	{
		cout << "No bus" << endl;
	}
}

void all_buses(map<string, vector<string>>& bus_stops)
{//вывести список всех маршрутов с остановками
	if (bus_stops.size()) // если есть маршруты
	{
		for (const auto& bus_pair : bus_stops) // получаем пары (маршрут - вектор остановок)
		{
			cout << "Bus " << bus_pair.first << ": ";
			for (const auto& stop : bus_pair.second) // получаем все остановки
			{
				cout << stop << " ";
			}
			cout << endl;
		}
	}
	else
	{
		cout << "No buses" << endl;
	}
}

int main()
{
	map<string, int> commands = { {"NEW_BUS", 0}, {"BUSES_FOR_STOP", 1}, {"STOPS_FOR_BUS", 2}, {"ALL_BUSES", 3} }; // карта команд
	map<string, vector<string>> bus_stops; // карта маршрутов
	map<string, vector<string>> stop_buses; // карта остановок
	int count; // количество команд
	string command; // команда ввода
	cin >> count;
	for (int i = 0; i < count; ++i)
	{
		cin >> command;
		auto action = commands.find(command); // поиск команды в карте команд
		string args1, args2; // аргументы команды
		int args3 = 0; 
		switch (action->second)
		{
			case(0): 
			{
				cin >> args1 >> args3; // bus stop_count (arguments)
				for (int i = 0; i < args3; ++i) // stop1 ... stopN
				{ // добавление данных в обе карты
					cin >> args2;
					new_bus(bus_stops, stop_buses, args1, args2);
				}
				break;
			}
			case(1):
			{ // вывести названия всех маршрутов через остановку
				cin >> args1; // stop
				buses_for_stop(stop_buses, args1);
				break;
			}
			case(2):
			{// вывести названия всех остановок маршрута bus со списком автобусов, на которые можно пересесть на каждой из остановок
				cin >> args1;
				stop_for_bus(bus_stops, stop_buses, args1);
				break;
			}
			case(3):
			{//вывести список всех маршрутов с остановками
				all_buses(bus_stops);
				break;
			}
		}
	}
	return 0;
}
