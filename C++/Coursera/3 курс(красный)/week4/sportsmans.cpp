#include <map>
#include <list>
#include <iostream>

using namespace std;

class CommandLine
{ // построенная шеренга спортсменов
	map<int, list<int>::const_iterator> numbers; // множество спортсменов, которые уже на поле (можно сделать vector<list<int>::const_iterator> (1'000'000, line.end())) будет делать за 0.02
	list<int> line; // построенная тренером шеренга
public:
	void add(int number, int next_number)
	{
		auto search = numbers.find(next_number); // ищем номер выданный тренером

		list<int>::iterator insert_it = (search == numbers.end())
			? line.insert(line.end(), number)  // если такого номера нет, вставляем в конец шеренги
			: line.insert(search->second, number);  // если был, ставим перед ним

		numbers[number] = insert_it; // обновляем множество
	}

	void print() const
	{
		for (int i : line)
		{
			cout << i << ' ';
		}
		cout << '\n';
	}
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	CommandLine my_team;
	int count;
	cin >> count;
	for (int i = 0; i < count; ++i)
	{
		int number, next_number;
		cin >> number >> next_number;
		my_team.add(number, next_number);
	}
	my_team.print(); // 0.08
	return 0;
}
