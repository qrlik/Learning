#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <sstream>

using namespace std;

/*
Напишите функцию FindNearestElement, для множества целых чисел numbers и данного числа border возвращающую итератор на элемент множества,
ближайший к border. Если ближайших элементов несколько, верните итератор на наименьший из них.
*/

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border)
{
	auto rhs = numbers.lower_bound(border); // первый элемент >= border
	if (rhs == numbers.begin())
	{  //если множество пустое либо если все элементы >= border
		return rhs;
	}
	auto lhs = prev(rhs); // иначе есть элемент меньше
	return (rhs == numbers.end() || border - *lhs <= *rhs - border) ? lhs : rhs;
  // либо >= элементов нет, либо левый элемент ближе
}

int main()
{
	set<int> numbers = { 1, 4, 6 };
	cout <<
		*FindNearestElement(numbers, 0) << " " <<
		*FindNearestElement(numbers, 3) << " " <<
		*FindNearestElement(numbers, 5) << " " <<
		*FindNearestElement(numbers, 6) << " " <<
		*FindNearestElement(numbers, 100) << endl;

	set<int> empty_set;

	cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
	return 0;
	/*
	1 4 4 6 6
	1
	*/
}
