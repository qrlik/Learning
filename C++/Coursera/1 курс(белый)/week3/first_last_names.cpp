#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
/*
Реализуйте класс для человека, поддерживающий историю изменений человеком своих фамилии и имени.
Считайте, что в каждый год может произойти не более одного изменения фамилии и не более одного изменения имени. При этом с течением времени могут открываться всё новые факты из прошлого человека, поэтому года́ в последовательных вызовах методов ChangeLastName и ChangeFirstName не обязаны возрастать.

Гарантируется, что все имена и фамилии непусты.

Строка, возвращаемая методом GetFullName, должна содержать разделённые одним пробелом имя и фамилию человека по состоянию на конец данного года.

Если к данному году не случилось ни одного изменения фамилии и имени, верните строку "Incognito".
Если к данному году случилось изменение фамилии, но не было ни одного изменения имени, верните "last_name with unknown first name".
Если к данному году случилось изменение имени, но не было ни одного изменения фамилии, верните "first_name with unknown last name".

В отличие от метода GetFullName, метод GetFullNameWithHistory должен вернуть не только последние имя и фамилию к концу данного года, но ещё и все
предыдущие имена и фамилии в обратном хронологическом порядке. Если текущие факты говорят о том, что человек два раза подряд изменил фамилию или имя
на одно и то же, второе изменение при формировании истории нужно игнорировать.
*/

class Person
{
private:
	map<int, string, greater<int>> m_first; // карта изменений имени по убыванию (для корректной работы lower_bound)
	map<int, string, greater<int>> m_last; // карта изменений фамилии по убыванию
public:
	void ChangeFirstName(int year, const string& first_name)
	{// добавить факт изменения имени на first_name в год year
		m_first[year] = first_name;
	}

	void ChangeLastName(int year, const string& last_name) 
	{// добавить факт изменения фамилии на last_name в год year
		m_last[year] = last_name;
	}

	vector<string> GetNamesHistory(const map<int, string, greater<int>>& map, int year) const
	{ // создает вектор истории имен начиная с последнего имени до самого старого
		vector<string> names;
		auto last = map.lower_bound(year); // //lower_bound для карты по убыванию - поиск первого ключа небольше чем year (key <= year), т.е самую свежую информацию
		for (; last != map.end(); ++last) // пока не получили конец карты
		{
			if (names.empty() || names.back() != last->second) // если пустой или последний элемент не повторяется
			{
				names.push_back(last->second); //заносим данные в вектор
			}
		}
		return names;
	}

	string BuildNamesHistory(const vector<string>& names, bool history) const // выводит все данные до year в формате "actual (old_data)"
	{
		string result = (!names.empty()) ? names[0] : ""; // добавляем последнее актуальное имя либо пустую строку
		if (history && names.size() > 1) // если выводим с историей
		{
			for (size_t i = 1; i < names.size(); ++i) // заполняем историю в скобках
			{
				result += (i == 1) ? " (" : ", "; // на первой итерации добавляем скобку
				result += names[i];
			}
			result += ")"; // закрываем скобку
		}
		return result;
	}

	string BuildFullName(int year, bool history) const // получает имя и фамилию, после анализа выводит результирующую строку
	{
		vector<string> names = GetNamesHistory(m_first, year); // вектор истории имен
		string first_names = BuildNamesHistory(names, history); // получаем строку имен (с историей / без истории)
		names = GetNamesHistory(m_last, year); // переписываем на вектор фамилий
		string last_names = BuildNamesHistory(names, history); // получаем строку фамилий
		if (!first_names.empty() && !last_names.empty())
		{ // есть фамилия и имя
			return first_names + " " + last_names;
		}
		else if (!first_names.empty() && last_names.empty())
		{ // есть имя, нет фамилии
			return first_names + " with unknown last name";
		}
		else if (first_names.empty() && !last_names.empty())
		{ // нет имени, есть фамилия
			return last_names + " with unknown first name";
		}
		else
		{ // нет ни имени, ни фамилии
			return "Incognito";
		}
	}

	string GetFullName(int year) const 
	{
		return BuildFullName(year, false);
	}

	string GetFullNameWithHistory(int year) const
	{
		return BuildFullName(year, true);
	}
};

int main() {
	//Person person;

	//person.ChangeFirstName(1900, "Eugene");
	//person.ChangeLastName(1900, "Sokolov");
	//person.ChangeLastName(1910, "Sokolov");
	//person.ChangeFirstName(1920, "Evgeny");
	//person.ChangeLastName(1930, "Sokolov");
	//cout << person.GetFullNameWithHistory(1940) << endl;
	////Evgeny (Eugene) Sokolov

	Person person;

	person.ChangeFirstName(1965, "Polina");
	person.ChangeLastName(1967, "Sergeeva");
	for (int year : {1900, 1965, 1990}) {
		cout << person.GetFullNameWithHistory(year) << endl;
	}
	//Incognito
	//Polina with unknown last name
	//Polina Sergeeva

	person.ChangeFirstName(1970, "Appolinaria");
	for (int year : {1969, 1970}) {
		cout << person.GetFullNameWithHistory(year) << endl;
	}
	//Polina Sergeeva
	//Appolinaria (Polina) Sergeeva

	person.ChangeLastName(1968, "Volkova");
	for (int year : {1969, 1970}) {
		cout << person.GetFullNameWithHistory(year) << endl;
	}
	//Polina Volkova (Sergeeva)
	//Appolinaria (Polina) Volkova (Sergeeva)

	person.ChangeFirstName(1990, "Polina");
	person.ChangeLastName(1990, "Volkova-Sergeeva");
	cout << person.GetFullNameWithHistory(1990) << endl;
	//Polina (Appolinaria, Polina) Volkova-Sergeeva (Volkova, Sergeeva)

	person.ChangeFirstName(1966, "Pauline");
	cout << person.GetFullNameWithHistory(1966) << endl;
	//Pauline (Polina) with unknown last name

	person.ChangeLastName(1960, "Sergeeva");
	for (int year : {1960, 1967}) {
		cout << person.GetFullNameWithHistory(year) << endl;
	}
	//Sergeeva with unknown first name
	//Pauline(Polina) Sergeeva

	person.ChangeLastName(1961, "Ivanova");
	cout << person.GetFullNameWithHistory(1967) << endl;
	//Pauline (Polina) Sergeeva (Ivanova, Sergeeva)
	return 0;
}
