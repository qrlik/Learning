#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
using namespace std;

struct inputControl
{
	int count = 0; // общее количество скобок
	vector<pair<string, bool>> commands; // команда + нужна ли скобка
	inputControl(int size) : commands(size) {};
	void read()
	{
		char prev_operation = '?'; // условный знак, чтобы не добавлять условие начала
		for (auto& [command, is_bracket] : commands)
		{
			getline(cin, command); // считываем операцию
			is_bracket = (command[0] == '*' || command[0] == '/') && (prev_operation == '-' || prev_operation == '+'); // единственный случай когда нужны скобки
			count += (is_bracket) ? 1 : 0; // счетчик скобок
			prev_operation = command[0]; // обновляем буфер
		}
	}
};

void printResult(const string& value, const inputControl& input_c)
{
	string bracket(input_c.count, '(');
	cout << bracket << value; // выводим все левые скобки
	for (const auto& [command, is_bracket] : input_c.commands)
	{ // печать остального выражения
		if (is_bracket)
		{
			cout << ')';
		}
		cout << ' ' << command;
	}
}

int main()
{
	string value; // первое значение
	int size; // количество операций
	cin >> value >> size;
	cin.ignore(1); // пропускаем первый \n после ввода size
	inputControl input(size);
	input.read(); // чтобы избежать рекурсивного перезаписывания строки, сначало все данные считываются, а потом выводятся
	printResult(value, input);
	return 0;
}
