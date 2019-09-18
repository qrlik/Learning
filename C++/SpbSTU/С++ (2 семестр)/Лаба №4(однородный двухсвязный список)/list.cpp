#include "list.h"
#include <fstream>
//реализация узла
Node::Node(Node* const p, const Circle& c): m_c(c), m_Prev(p), m_Next(p->m_Next) // добавление объекта следом за Node* p
{ 
	p->m_Next = this; // замена боковых указателей
	m_Next->m_Prev = this;
}

Node::~Node()
{
	if (m_Next) // проверка на конец списка
	{
		m_Next->m_Prev = m_Prev;
	}
	if (m_Prev) // проверка на начало списка
	{
		m_Prev->m_Next = m_Next;
	}
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
	os << n.m_c;
	return os;
}

//реализация списка
List::List() : m_size(0) // по умолчанию замыкаем голову и хвост
{
	Head.m_Next = &Tail;
	Tail.m_Prev = &Head;
}

List::List(const List& other): m_size(0)
{
	Head.m_Next = &Tail; // замыкаемся для корректного копирования
	Tail.m_Prev = &Head;
	*this = other; // оператор копирования
}

List::List(List&& other)// копируются указатели головы,хвоста
{
	Head.m_Next = &Tail; // замыкаемся для корректного копирования
	Tail.m_Prev = &Head;
	*this = std::move(other); // move-оператор копирования
}

List& List::operator=(const List& other)
{
	Node* tmp1 = Head.m_Next; // указатель для 1 списка
	Node* tmp2 = other.Head.m_Next; // указатель для 2 списка
	while (tmp1 != &Tail && tmp2 != &other.Tail) // пока один из указателей не упрется в хвост копируем узлы
	{
		tmp1->m_c = tmp2->m_c;
		tmp1 = tmp1->m_Next;
		tmp2 = tmp2->m_Next;
	}
	if (tmp1 = &Tail) // если 1 список оказался меньше, добавляем узлы
	{
		while (tmp2 != &other.Tail)
		{
			addToTail(tmp2->m_c);
			tmp2 = tmp2->m_Next;
		}
	}
	else // если 1 список оказался больше, удаляем узлы
	{
		while (tmp1 != &Tail)
		{
			Node* tmp = tmp1->m_Next;
			delete tmp1;
			m_size--; // корректируем размер
			tmp1 = tmp;
		}
	}
	return *this;
}

List& List::operator=(List&& other)
{
	clean(); // очищается память
	if (other.m_size)
	{
		other.Head.m_Next->m_Prev = &Head; // меняем указатель 1 узла
		Head.m_Next = other.Head.m_Next; // меняем указатель Head
		other.Head.m_Next = &other.Tail; // замыкаем other.Head
		other.Tail.m_Prev->m_Next = &Tail; // меняем указатель последнего узла
		Tail.m_Prev = other.Tail.m_Prev; // меняем указатель Tail
		other.Tail.m_Prev = &other.Head; // зацикливаем хвост
	}
	m_size = other.m_size;
	other.m_size = 0;
	return *this;
}

List::~List()
{
	clean(); // очищается память
}

void List::addToHead(const Circle& c)
{
	new Node(&Head, c);
	m_size++;
}

void List::addToTail(const Circle& c)
{
	new Node(Tail.m_Prev, c);
	m_size++;
}

void List::addToTail(Node* n)
{
	n->m_Prev = Tail.m_Prev;
	n->m_Next = &Tail;
	Tail.m_Prev->m_Next = n;
	Tail.m_Prev = n;
	m_size++;
}

bool List::remove(const Circle& c) // удаление первого подходящего элемента
{
	Node* srch = Head.m_Next;
	while (srch != &Tail)
	{
		if (srch->m_c == c)
		{
			delete srch;
			m_size--;
			return true;
		}
		srch = srch->m_Next;
	}
	return false;
}

int List::removeEvery(const Circle& c) // удаление всех подходящих элементов
{
	Node* srch = Head.m_Next;
	int count = 0;
	while (srch != &Tail)
	{
		Node* tmp = srch->m_Next;
		if (srch->m_c == c)
		{
			delete srch;
			count++;
		}
			srch = tmp;
	}
	m_size -= count; // уменьшаем size
	return count;
}

void List::clean()
{
	while (Head.m_Next != &Tail)
	{
		delete Head.m_Next; // вызывается деконструктор Node и сам перенаправляет все указатели
	}
	m_size = 0;
}

void List::sort()
{
	List tmp; // буферные голова,хвост
	int min = 0; // минимальный радиус
	Node* srch = Head.m_Next; // указатель для поиска минимума
	Node* pmin = nullptr; // указатель на минимум
	for (size_t i = 0; i < m_size; i++) // сортировка, в конце останутся только голова и хвост
	{
		min = Head.m_Next->m_c.GetR(); // с первого узла считываем радиус
		pmin = Head.m_Next;
		srch = Head.m_Next->m_Next; // направляем указатель поиска на следующий
		for (size_t j = 0; j < m_size - i - 1; j++) // ищем минимум, -1 потому что первый элемент уже занесен, начинаем со второго
		{
			if (min > srch->m_c.GetR())
			{
				min = srch->m_c.GetR();
				pmin = srch;
			}
			srch = srch->m_Next;
		}
		pmin->m_Prev->m_Next = pmin->m_Next; // убираем узел из списка
		pmin->m_Next->m_Prev = pmin->m_Prev;
		tmp.addToTail(pmin); // вносим узел в конец буферного списка
	} // имеем пустой исходный список и отсортированный буферный
	*this = std::move(tmp);
}

void List::writeToFile(const char* f) const
{
	if (f)
	{
		std::ofstream fout;
		fout.open(f); // привязка объекта к файлу
		if (fout.is_open())
		{
			fout << *this;
		}
		fout.close(); // закрываем файл
	}
	else
	{
		std::cout << "Wrong file" << std::endl;
	}
}

void List::readFromFile(const char* f)
{
	if (f)
	{
		std::ifstream fin;
		fin.open(f); // открываем
		if (fin.is_open())
		{
			int x = 0;
			int y = 0;
			int r = 0;
			fin.ignore(100, '('); // 1. Point: (6, 3) Radius: 1
			while (fin >> x)
			{
				fin.ignore(100, ',');
				fin >> y;
				fin.ignore(100, ':');
				fin >> r;
				fin.ignore(100, '(');
				Circle c(x, y, r);
				addToTail(c);
			}
		}
		fin.close(); // закрываем
	}
	else
	{
		std::cout << "Wrong file" << std::endl;
	}
}

std::ostream& operator<<(std::ostream& os, const List& bd)
{
	Node* tmp = bd.Head.m_Next;
	size_t count = 1;
	for (size_t i = 0; i < bd.m_size; i++)
	{
		os << count++ << ". " << *tmp << std::endl;
		tmp = tmp->m_Next;
	}
	return os;
}
