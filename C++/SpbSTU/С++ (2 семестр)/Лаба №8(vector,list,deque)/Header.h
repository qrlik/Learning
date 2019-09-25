#pragma once
#include <vector>
using namespace std;

template<typename T> void print_info(const vector<T>& v)
{
	cout << "Size:\t" << v.size() << " Capacity:\t" << v.capacity() << " Max_size:\t" << v.max_size() << endl;
	typename vector<T>::const_iterator itb = v.begin(), ite = v.end();
	while (itb != ite)
	{
		cout << *itb << " ";
		++itb;
	}
	cout << endl;
}

template<typename T> void print_info(const vector<T*>& v)
{
	cout << "Size:\t" << v.size() << " Capacity:\t" << v.capacity() << " Max_size:\t" << v.max_size() << endl;
	typename vector<T*>::const_iterator itb = v.begin(), ite = v.end();
	while (itb != ite)
	{
		cout << *(*itb) << " ";
		++itb;
	}
	cout << endl;
}

template<typename T> std::ostream& operator<<(std::ostream& os, const vector<T>& vv)
{
	for (int i = 0; i < vv.size(); i++)
	{
		os << vv[i] << " ";
	}
	os << endl;
	return os;
}

void insertChar(vector<char>& vc, char insert)
{
	for (int i = 0; i < vc.size(); i++)
	{
		if (vc[i] == insert)
		{
			return;
		}
	}
	vc.insert(vc.begin(), insert);
}

void noPair(vector<char>& str)
{
	vector<char>::iterator itb = str.begin();
	vector<char>::iterator start = itb;
	while (itb != str.end() - 1) // -1 чтобы не выйти за пределы
	{
		if (*itb == *(itb + 1))
		{
			start = itb;
			while (*itb == *(itb + 1)) // находим диапазон дублей
			{
				++itb;
			}
			++itb; // делаем последнюю итерацию хвоста
			itb = str.erase(start, itb); // удаляем весь диапазон
		}
		else
		{
			++itb;
		}
	}
}

template<typename T> void noDublies(vector<T>& v)
{
	vector<char>::iterator it = v.begin();
	vector<char>::iterator search = it;
	while (it != v.end() - 1) // -1 чтобы не выйти за пределы
	{
		search = it + 1; // начинаем искать со следующего
		do
		{
			if (*search == *it) // удаляем все дубли
			{
				search = v.erase(search);
			}
			else
			{
				++search;
			}
		} while (search != v.end());
		++it;
	}
}

template<typename C> void print_container(const C& c)
{
	cout << typeid(C).name() << endl;
	typename C::const_iterator itb = c.begin(), ite = c.end();
	while (itb != ite)
	{
		cout << *itb << " | ";
		++itb;
	}
	cout << endl;
}
