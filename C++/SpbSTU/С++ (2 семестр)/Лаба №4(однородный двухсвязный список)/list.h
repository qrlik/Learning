#pragma once
#include "circle.h"
#include <iostream>
// интерфейс узла
class Node
{
	Circle m_c;
	Node* m_Prev, *m_Next;
	Node() : m_c(), m_Prev(nullptr), m_Next(nullptr) {}
	Node(Node* const p, const Circle& c);
	~Node();
	friend class List;
	friend std::ostream& operator<<(std::ostream& os, const Node& n);
	friend std::ostream& operator<<(std::ostream& os, const List& bd);
};
// интерфейс списка
class List
{
	Node Head, Tail;
	size_t m_size;
	void addToTail(Node* n);
public:
	List();
	List(const List& other);
	List(List&& other);
	List& operator=(const List& other);
	List& operator=(List&& other);
	~List();
	void addToHead(const Circle& c);
	void addToTail(const Circle& c);
	bool remove(const Circle& c);
	int removeEvery(const Circle& c);
	void clean();
	void sort();
	void writeToFile(const char* f) const;
	void readFromFile(const char* f);
	friend std::ostream& operator<<(std::ostream& os, const List& bd);
};

std::ostream& operator<<(std::ostream& os, const Node& n);
std::ostream& operator<<(std::ostream& os, const List& bd);
