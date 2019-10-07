#include <iostream>
#include <string>
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
	string GetFullName(int year) // получить имя и фамилию по состоянию на конец года year
	{// lower_bound для карты по убыванию - поиск первого ключа небольше чем year (key <= year), т.е самую свежую информацию
		auto first_name = m_first.lower_bound(year); // пара год-имя
		auto last_name = m_last.lower_bound(year); // пара год-фамилия
		if (first_name != m_first.end() && last_name != m_last.end())
		{ // есть фамилия и имя
			return first_name->second + " " + last_name->second;
		}
		else if (first_name != m_first.end() && last_name == m_last.end())
		{ // есть имя, нет фамилии
			return first_name->second + " with unknown last name";
		}
		else if (first_name == m_first.end() && last_name != m_last.end())
		{ // нет имени, есть фамилия
			return last_name->second + " with unknown first name";
		}
		else
		{ // нет ни имени, ни фамилии
			return "Incognito";
		}
	}
};

int main() 
{
	Person person;

	person.ChangeFirstName(1965, "Polina");
	person.ChangeLastName(1967, "Sergeeva");
	for (int year : {1900, 1965, 1990}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeFirstName(1970, "Appolinaria");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeLastName(1968, "Volkova");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	/*
	Incognito
	Polina with unknown last name
	Polina Sergeeva
	Polina Sergeeva
	Appolinaria Sergeeva
	Polina Volkova
	Appolinaria Volkova
	*/

	return 0;
}
