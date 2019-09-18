#include "list.h"
#include <fstream>
#include <typeinfo>

//реализация Node
Node::Node(Node* const pn, const Shape& ps) : pShape(ps.make()), m_Prev(pn), m_Next(pn->m_Next) // добавление объекта следом за Node* p
{
	pn->m_Next = this; // замена боковых указателей
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
	delete pShape;
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
	if (n.pShape)
	{
		n.pShape->print(os);
	}
	return os;
}

//реализация List
List::List() :m_size(0) // по умолчанию замыкаем голову и хвост
{
	Head.m_Next = &Tail;
	Tail.m_Prev = &Head;
}

List::~List()
{
	clean(); // очищаем лист
}

List::List(const List& other) : m_size(0)
{
	Head.m_Next = &Tail;
	Tail.m_Prev = &Head;
	*this = other; // оператор копирования
}

List::List(List&& other) : m_size(other.m_size)
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
		if (typeid(*tmp1) == typeid(*tmp2)) // если типы совпадают, вызываем оператор присваивания по умолчанию
		{
			*(tmp1->pShape) = *(tmp2->pShape);
		}
		else
		{
			delete tmp1->pShape; // удаляем только данные узла
			tmp1->pShape = tmp2->pShape->make(); // создаем копию нужного типа
		}
		tmp1 = tmp1->m_Next;
		tmp2 = tmp2->m_Next;
	}
	if (tmp1 = &Tail) // если 1 список оказался меньше, добавляем узлы
	{
		while (tmp2 != &other.Tail)
		{
			addToTail(*(tmp2->pShape));
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
	clean();  // очищается память
	if (other.m_size)
	{
		other.Head.m_Next->m_Prev = &Head; // меняем указатель 1 узла
		Head.m_Next = other.Head.m_Next; // меняем указатель Head
		other.Head.m_Next = &other.Tail; // замыкаем other.Head
		other.Tail.m_Prev->m_Next = &Tail; // меняем указатель последнего узла
		Tail.m_Prev = other.Tail.m_Prev; // меняем указатель Tail
		other.Tail.m_Prev = &other.Head; // зацикливаем хвост,голову исходника
	}
	m_size = other.m_size;
	other.m_size = 0;
	return *this;
}

void List::addToHead(const Shape& ps)
{
	new Node(&Head, ps);
	m_size++;
}

void List::addToTail(const Shape& ps)
{
	new Node(Tail.m_Prev, ps);
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

bool List::remove(const Shape& ps) // удаление первого подходящего элемента
{
	Node* srch = Head.m_Next;
	while (srch != &Tail)
	{
		if (*(srch->pShape) == ps) // вызывается == типа объекта srch->pShape, сравнение базы, потом типа с ps, потом начинки
		{
			delete srch;
			m_size--;
			return true;
		}
		srch = srch->m_Next;
	}
	return false;
}

int List::removeEvery(const Shape& ps) // удаление всех подходящих элементов
{
	Node* srch = Head.m_Next;
	int count = 0;
	while (srch != &Tail)
	{
		Node* tmp = srch->m_Next;
		if (*(srch->pShape) == ps)
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

void List::sort(int choise)
{
	//	List tmp; // буферные голова,хвост
	Node* now = Head.m_Next; // текущая позиция поиска
	Node* pmin = nullptr; // указатель на минимум
	Node* srch = nullptr; // указатель для поиска минимума
	bool (Shape::*pcompare)(const Shape& rhs) = nullptr; // указатель на нужный метод сравнения
	switch (choise)
	{
	case(1): // defaul + case(1)
	default:
		pcompare = &Shape::compareCol;
		break;
	case(2):
		pcompare = &Shape::compareSqr;
		break;
	case(3):
		pcompare = &Shape::compareDst;
		break;
	}
	for (size_t i = 0; i < m_size - 1; i++) // сортировка, в конце останутся только голова и хвост
	{
		pmin = now;
		srch = now->m_Next; // направляем указатель поиска на следующий
		for (size_t j = 0; j < m_size - i - 1; j++) // ищем минимум, -1 потому что первый элемент уже занесен, начинаем со второго
		{
			if ((pmin->pShape->*pcompare)(*(srch->pShape)))
			{
				pmin = srch;
			}
			srch = srch->m_Next;
		}
		if (now != pmin)
		{
			Shape* tmp = now->pShape; // буферный список не нужен, т.к мы просто меняем указатели на начинку Node
			now->pShape = pmin->pShape;
			pmin->pShape = tmp;
		}
		now = now->m_Next;
		//pmin->m_Prev->m_Next = pmin->m_Next; // убираем узел из списка
		//pmin->m_Next->m_Prev = pmin->m_Prev;
		//tmp.addToTail(pmin); // вносим узел в конец буферного списка
	} // имеем пустой исходный список и отсортированный буферный
//	*this = std::move(tmp);
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
			double x1 = 0;
			double x2 = 0;
			double y1 = 0;
			double y2 = 0;
			int col = 0;
			char buf[10];
			fin.ignore(100, '.'); // 1. Circle: (1, 2, 3, 1) 2. Rect: (0, 0, 0, 0, 0)
			while (fin >> buf)
			{
				fin.ignore(100, '(');
				fin >> x1;
				fin.ignore(100, ',');
				fin >> x2;
				fin.ignore(100, ',');
				fin >> y1;
				fin.ignore(100, ',');
				if (!strcmp(buf, "Circle:"))
				{
					fin >> col;
					addToTail(Circle (x1, x2, y1, static_cast<COLOR>(col)));
				}
				else
				{
					fin >> y2;
					fin.ignore(100, ',');
					fin >> col;
					addToTail(Rect (x1, x2, y1, y2, static_cast<COLOR>(col)));
				}
				fin.ignore(100, '.');
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
