#include <iostream>
#include <string>
#include <map>
using namespace std;

void change_capital(map<string,string>& world, const string& country, const string& new_capital)
{
	if (world.count(country) == 0) // если страны нету
	{
		cout << "Introduce new country " << country << " with capital " << new_capital << endl;
	}
	else // если страна уже присутствовала
	{
		const string& old_capital = world[country]; // вытаскиваем через итератор старую столицу
		if (old_capital != new_capital) // сравниваем старую и новую строку
		{
			cout << "Country " << country << " has changed its capital from " << old_capital << " to " << new_capital << endl;
		}
		else
		{
			cout << "Country " << country << " hasn't changed its capital" << endl;
		}
	}
	world[country] = new_capital; // заносим страну / меняем столицу
}

void rename(map<string, string>& world, const string& old_country_name, const string& new_country_name)
{
	//auto search_old = world.find(old_country_name);
	//auto search_new = world.find(new_country_name);
	if (world.count(old_country_name) == 0 || world.count(new_country_name) == 1) // если старый не существует или существует новый
	{
		cout << "Incorrect rename, skip" << endl;
	}
	else
	{
		world[new_country_name] = world[old_country_name]; // заносим новый ключ и старое значение
		world.erase(old_country_name); // удаляем старый
		cout << "Country " + old_country_name + " with capital " + world[new_country_name] + " has been renamed to " + new_country_name << endl;
	}
}

void about(const map<string, string>& world, const string& country)
{
	auto search = world.find(country);
	if (search != world.end())
	{
		cout << "Country " + country + " has capital " + search->second << endl;
	}
	else
	{
		cout << "Country " << country << " doesn't exist" << endl;
	}
}

void dump(const map<string, string>& world)
{
	if (world.size())
	{
		for (const auto& pair : world)
		{
			cout << pair.first << "/" << pair.second << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "There are no countries in the world" << endl;
	}
}

int main()
{
	map<string, int> commands = { {"CHANGE_CAPITAL", 0}, {"RENAME", 1}, {"ABOUT", 2}, {"DUMP", 3} };
	map<string, string> world;
	int count;
	string command;
	cin >> count;
	for (int i = 0; i < count; ++i)
	{
		cin >> command;
		auto action = commands.find(command);
		string args1, args2;
		switch (action->second)
		{
			case(0):
			{
				cin >> args1 >> args2;
				change_capital(world, args1, args2);
				break;
			}
			case(1):
			{
				cin >> args1 >> args2;
				rename(world, args1, args2);
				break;
			}
			case(2):
			{
				cin >> args1;
				about(world, args1);
				break;
			}
			case(3):
			{
				dump(world);
				break;
			}
		}
	}
	return 0;
}
