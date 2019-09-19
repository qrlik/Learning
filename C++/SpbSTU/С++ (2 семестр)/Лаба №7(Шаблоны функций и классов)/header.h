#pragma once

template<typename T>void Swap(T& a, T& b)
{
	T c = std::move(a); // используем move-operator=
	a = std::move(b); // на случай сложный объектов, чтобы лишний раз не выделять память
	b = std::move(c);
}

template<typename T, const size_t size = 10> class MyStack
{
	T ar[size];
	int pos = 0; // позиция первого пустого элемента
public:
	void push(const T& object) // добавление в конец
	{
		if (pos == size) // если стэк полный
		{
			throw std::out_of_range("Stack is full");
		}
		ar[pos++] = object; // копируем объект в стэк и увеличиваем pos
	}
	const T pop() // изъятие с конца (можно использовать только справа от '=' )
	{
		if (pos == 0) // если стэк пустой
		{
			throw std::out_of_range("Stack is empty");
		}
		return ar[--pos]; // сначала уменьшаем pos и возвращаем копию ненужного объекта
	}
	T& operator[](const int& i)
	{
		if (i >= 0 && i < pos) // диапазон значений, сформированных пользователем
		{
			return ar[i];
		}
		throw std::out_of_range("Out of range");
	}
	const T& operator[](int i) const // вариант для const-объектов
	{
		if (i >= 0 && i < pos) // диапазон значений, сформированных пользователем
		{
			return ar[i];
		}
		throw std::out_of_range("Out of range");
	}
};

template<typename T> class Node
{
	T data;
	Node<T>* p_Next;
	Node(): data(), p_Next(nullptr) {}
	Node(const T& other) : data(other), p_Next(nullptr) {}
	Node(T&& other) : data(std::move(other)), p_Next(nullptr) {}
	template <typename T> friend class MyStack2;
};

template<typename T> class MyStack2
{
	Node<T>* Head; // указатель на 1 элемент списка (можно сделать статическим и только будет один список для одного типа данных)
	int count; // количество элементов
public:
	MyStack2(): Head(nullptr), count(0) {}
	void push(const T& object) // добавление в конец
	{
		Node<T>* tmp = new Node<T>(object); // выделяем память под новый элемент
		tmp->p_Next = Head; // направляем указатели
		Head = tmp;
		count++; // увеличиваем размер
	}
	const T pop() // изъятие с головы
	{
		if (count == 0)
		{
			throw std::out_of_range("Stack is empty");
		}
		T tmp_data = Head->data;
		Node<T>* tmp_pointer = Head->p_Next; // указатель на второй
		delete Head; // удалили первый
		Head = tmp_pointer; // поменяли указатель на второй
		count--; // уменьшаем размер
		return tmp_data;
	}
	T& operator[](const int& i)
	{
		if (i >= 0 && i < count) // диапазон значений, сформированных пользователем
		{
			Node<T>* search = Head;
			for (int j = 0; j < i; j++)
			{
				search = search->p_Next;
			}
			return search->data;
		}
		throw std::out_of_range("Out of range");
	}
	const T& operator[](int i) const // для const-объектов
	{
		if (i >= 0 && i < count) // диапазон значений, сформированных пользователем
		{
			Node<T>* search = Head;
			for (int j = 0; j < i; j++)
			{
				search = search->p_Next;
			}
			return search->data;
		}
		throw std::out_of_range("Out of range");
	}
	void print() const // т.к эффективно добавляем в начало, печать в порядке добавления будет вызываться неэффективно(либо добавлять надо в хвост, но добавляем и удаляем обычно чаще чем печатаем)
	{
		for (int i = count; i > 0; i--)
		{
			Node<T>* print = Head;
			for (int j = 0; j < i - 1; j++) // -1 т.к указатель уже на первом элементе
			{
				print = print->p_Next;
			}
			std::cout << print->data << std::endl; // печать с конца
		}
	}
};

template<typename T> class MyQueue
{
	size_t m_cap; // емкость
	size_t m_n; // количество элементов
	size_t m_first; // первый элемент
	size_t m_last; // последний пустой m_last = (m_first + m_n) % m_cap
	T* ar;
public:
	MyQueue(size_t size = 10) : m_cap(size), m_n(0), m_first(0), m_last(0) { ar = new T[size]; }
	~MyQueue() { delete[] ar; }
	MyQueue(const MyQueue& other) : m_cap(other.m_n), m_n(other.m_n), m_first(0), m_last(other.m_n)
	{
		ar = new T[m_n];
		for (size_t i = 0; i < m_n; i++) // перезапись
		{
			ar[i] = other.ar[(other.m_first + i) % other.m_cap]; // перезапись очереди
		}
	}
	MyQueue(MyQueue&& other) : m_cap(other.m_cap), m_n(other.m_n), m_first(other.m_first), m_last(other.m_last), ar(other.ar)
	{
		other.ar = nullptr; // можно заменить на move-operator=, но со списком инициализации по-красивей
		other.m_cap = 0;
		other.m_n = 0;
		other.m_first = 0;
		other.m_last = 0;
	}
	MyQueue& operator=(const MyQueue& other)
	{
		if(m_cap < other.m_n) // если очереди нехватает, выделяем память
		{
			delete[] ar;
			ar = new T[other.m_n];
			m_cap = other.m_n;
		}
		for (size_t i = 0; i < other.m_n; i++) // перезапись
		{
			ar[i] = other.ar[(other.m_first + i) % other.m_cap];
		}
		m_n = other.m_n;
		m_first = 0;
		m_last = m_n;
	}
	MyQueue& operator=(MyQueue&& other)
	{
		ar = other.ar;
		m_cap = other.m_cap;
		m_n = other.m_n;
		m_first = other.m_first;
		m_last = other.m_last;
		other.ar = nullptr;
		other.m_cap = 0;
		other.m_n = 0;
		other.m_first = 0;
		other.m_last = 0;
	}
	void push(const T& object)
	{
		if (m_n == m_cap) // если заполнен
		{
			addCap();
		}
		ar[m_last] = object;
		m_n++;
		m_last = (m_first + m_n) % m_cap;
	}
	const T pop() // удаляем первый
	{
		if (ar == nullptr) // мало ли
		{
			throw std::exception("Queue is nullptr");
		}
		else if (m_n == 0)
		{
			throw std::exception("Queue is empty");
		}
		size_t m_firstBuf = m_first;
		m_n--;
		m_first = (m_last + m_cap - m_n) % m_cap;
		return ar[m_firstBuf];
	}
	void addCap()
	{
		T* newAr = new T[m_cap + 5]; // увеличение емкости очереди
		for (size_t i = 0; i < m_n; i++) // перезапись
		{
			newAr[i] = std::move(ar[(m_first + i) % m_cap]);
		}
		delete[] ar; // очистили старую память
		ar = newAr;
		m_cap += 5; // увеличение емкости
		m_first = 0;
		m_last = m_n;
	}
	T& operator[](const int& i)
	{
		if (i >= 0 && i < m_n) // диапазон значений, сформированных пользователем
		{
			return ar[(m_first + i) % m_cap];
		}
		throw std::exception("Out of range");
	}
	const T& operator[](const int& i) const
	{
		if (i >= 0 && i < m_n) // диапазон значений, сформированных пользователем
		{
			return ar[(m_first + i) % m_cap];
		}
		throw std::exception("Out of range");
	}
};
