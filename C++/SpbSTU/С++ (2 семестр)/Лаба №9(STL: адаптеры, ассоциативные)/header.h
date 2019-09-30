#pragma once
using namespace std;

template<typename C> void print_container(const C& c) // шаблон для печати базовых контейнеров
{
	typename C::const_iterator itb = c.begin(), ite = c.end();
	while (itb != ite)
	{
		cout << *itb << " | ";
		++itb;
	}
	cout << endl;
}

template <typename C> const typename C::value_type& getTop(const C& c) // получение элемента для stack/priority_queue
{
	return c.top();
}

template <typename T1, typename T2> const T1& getTop(const queue<T1, T2>& q) // получение элемента для queue
{
	return q.front();
}

template <typename C> void print_adapter(C c) // шаблон для печати адаптеров
{
	if (c.size())
	{
		cout << typeid(C).name() << endl;

		while (c.size())
		{
			cout << getTop(c) << " | ";
			c.pop();
		}
		cout << endl;
	}
	else cout << "Container is empty" << endl;
}

struct my_compare // предикат для строк
{
	bool operator()(const char* lhm, const char* rhm) const
	{
		return strcmp(lhm, rhm) < 0;
	}
};
// следующие три шаблона можно объединить в один, но я пока не знаю как на этапе компиляции различать какой тип(примитивный или др.)
// поступил в шаблон
template <typename T1, typename T2, typename T3> void print_map(const map<T1, T2, T3>& m) // обычная печать словаря
{
	typename map<T1, T2>::const_iterator itb = m.cbegin(), ite = m.cend();
	cout << typeid(m).name() << endl;
	while (itb != ite)
	{
		cout << "Key: " << itb->first << " | Value: " << itb->second << endl;
		++itb;
	}
	cout << endl;
}

template <typename T1, typename T2> void print_map(const map<T1, set<T2>>& m) // печать словаря с set
{
	typename map<T1, set<T2>>::const_iterator itb = m.cbegin(), ite = m.cend();
	cout << typeid(m).name() << endl;
	while (itb != ite)
	{
		cout << "Key: " << itb->first << " | Value: ";
		print_container(itb->second);
		++itb;
	}
	cout << endl;
}

template <typename T1, typename T2> void print_map(const map<T1, multiset<T2>>& m) // печать словаря с multiset (не понял как соединить)
{
	typename map<T1, multiset<T2>>::const_iterator itb = m.cbegin(), ite = m.cend();
	cout << typeid(m).name() << endl;
	while (itb != ite)
	{
		cout << "Key: " << itb->first << " | Value: ";
		print_container(itb->second);
		++itb;
	}
	cout << endl;
}

template <typename T> void print_multimap(const multimap<T, pair<T, T>>& m)
{
	typename multimap<T, pair<T, T>>::const_iterator itb = m.cbegin(), ite = m.cend();
	cout << typeid(m).name() << endl;
	while (itb != ite)
	{
		wcout << L"Key: " << itb->first << L" | Value: " << itb->second.first << L", " << itb->second.second << endl;
		++itb;
	}
	cout << endl;
}

template <typename T> void print_mmkey(const multimap<T, pair<T, T>>& m, const T key)
{
	if (m.size())
	{
		typename multimap<T, pair<T, T>>::const_iterator itb = m.lower_bound(key); // поиск первого совпадения
		if (itb != m.end())
		{
			wcout << L"Key: " << itb->first << L"| Value: ";
			typename multimap<T, pair<T, T>>::const_iterator ite = m.upper_bound(key); // поиск первого не совпадения
			while (itb != ite) // печать всего диапазона нужных значений
			{
				wcout << itb->second.first << L" " << itb->second.second << L" ";
				++itb;
			}
			cout << endl;
		}
		else
		{
			cout << "No such word" << endl;
		}
	}
	else
	{
		cout << "Dictionary is empty" << endl;
	}
}
