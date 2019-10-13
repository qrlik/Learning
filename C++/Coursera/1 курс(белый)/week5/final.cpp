#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <set>
using namespace std;

map<string, int> commands = { {"Add", 1}, {"Del", 2}, {"Find", 3}, {"Print", 4} }; // карта комманд

class Date
{
private:
	int m_year;
	int m_month;
	int m_day;
public:
	Date(const string& str)
	{ // в конструкторе лучше не кидать исключения, особенно в сложных классах. можно заменить на метод Database, или на функцию
		stringstream stream(str);
		int y, m, d;
		char c1, c2;
		stream >> y >> c1 >> m >> c2 >> d; // считываем данные
		if (stream && stream.eof() && c1 == '-' && c2 == '-') // если считалось корректорно
		{// 1. была ли запись во все переменные 2. далее ничего не было 3. верные разделители
			if (m < 1 || m > 12)
			{ // проверяем месяц
				throw invalid_argument("Month value is invalid: " + to_string(m));
			}
			else if (d < 1 || d > 31)
			{ // проверяем дату
				throw invalid_argument("Day value is invalid: " + to_string(d));
			} // задаем переменные
			m_year = y;
			m_month = m;
			m_day = d;
		}
		else
		{
			throw invalid_argument("Wrong date format: " + str);
		}
	}
	int GetYear() const { return m_year; }
	int GetMonth() const { return m_month; }
	int GetDay() const { return m_day; }
	bool operator<(const Date& rhs) const
	{ // т.к значение года проверять отдельно не нужно, не нужно проверять календарную корректность даты, високосные года учитывать не нужно
		if (m_year == rhs.m_year)
		{ // рассматриваем единственный случай сравнения не по году, а по количеству дней в году
			return m_month * 31 + m_day < rhs.m_month * 31 + rhs.m_day;
		} // можно использовать два вектора
		return m_year < rhs.m_year;
	}
};

ostream& operator<<(ostream& os, const Date& date) noexcept
{ // Вам не понадобится выводить дату с отрицательным значением года
	os << setfill('0') << setw(4) << date.GetYear() << '-'
		<< setw(2) << date.GetMonth() << '-'
		<< setw(2) << date.GetDay();
	return os;
}

class Database
{
private:
	map<Date, set<string>> m_data;
public:
	void add(const Date& date, const string& event) noexcept
	{// метод добавления события по дате
		m_data[date].insert(event);
	}
	bool del(const Date& date, const string& event) noexcept
	{// метод удаления события по дате
		if (m_data.count(date) && m_data[date].erase(event))
		{// если есть такая дата и если удалили
			cout << "Deleted successfully" << endl;
			if (m_data[date].empty())
			{ // если список событий стал пустым, удаляем дату
				m_data.erase(date);
			}
			return true;
		}//если нечего удалять
		cout << "Event not found" << endl;
		return false;
	}
	int del(const Date& date) noexcept
	{// возвращает количество удаленных событий
		int result = 0;
		if (m_data.count(date))
		{// если дата присутствует
			result = m_data[date].size();
			m_data[date].clear(); // очищаем дату
			m_data.erase(date); // удаляем дату
		}
		cout << "Deleted " << result << " events" << endl;
		return result;
	}
	void find(const Date& date) const
	{//возвращает множество событий
		try
		{
			for (const string& event : m_data.at(date))
			{ // вывод по возрастанию
				cout << event << endl;
			}
		}
		catch (out_of_range & ex)
		{ // если обратились за пределы карты, игнорируем
			return;
		}
	}
	void print() const noexcept
	{
		for (const auto& [date, set] : m_data)
		{ // итерируемся по датам
			for (const string& event : set)
			{ // итерируемся по событиям даты
				cout << date << ' ' << event << endl;
			}
		}
	}
};

int main() 
{
	Database events;
	string income_line;
	while (getline(cin, income_line)) // получаем команду
	{
		stringstream command_stream(income_line); // заносим в буффер для удобства
		string command;
		command_stream >> command; // получаем команду
		try
		{
			if (command.empty())
			{// если получили пустую строку, игнорируем
				continue;
			}
			else if (commands.count(command) == 0)
			{ // команда не определена
				throw domain_error("Unknown command: " + command); // ошибка команды
			}
			else
			{// нашли команду
				string date_str, event;
				command_stream >> date_str >> event; // получили дату и событие(если есть)
				switch (commands[command]) // получаем номер команды. at() не нужен, т.к выше проверка
				{
				case(1): // добавление события
					events.add(date_str, event);
					break;
				case(2): // удаление по дате . вызываем соответствующий метод
					(event.empty()) ? events.del(date_str) : events.del(date_str, event);
					break;
				case(3): // поиск по дате
					events.find(date_str);
					break;
				case(4): // печать
					events.print();
					break;
				}
			}
		}
		catch (logic_error & ex)
		{
			cout << ex.what() << endl;
			break; // "После обработки любой из описанных ошибок ввода и печати сообщения программа должна завершать своё выполнение"
		}
		catch (exception & ex)
		{
			cout << "Another exception: " << ex.what() << endl;
		}
	}
	return 0;
}
