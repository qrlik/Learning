#pragma once
#include <iostream>
#include "shape.h"

class Node
{
	Shape* pShape;
	Node* m_Prev, *m_Next;
	Node() :m_Prev(nullptr), m_Next(nullptr), pShape(nullptr) {}
	Node(Node* const pn, const Shape& ps);
	~Node();
	friend class List;
	friend std::ostream& operator<<(std::ostream& os, const Node& n);
	friend std::ostream& operator<<(std::ostream& os, const List& bd);
};

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
	void addToHead(const Shape& ps);
	void addToTail(const Shape& ps);
	bool remove(const Shape& ps);
	int removeEvery(const Shape& ps);
	void clean();
	void sort(int choise); // 1 - color, 2 - square, 3 - distance
	void writeToFile(const char* f) const;
	void readFromFile(const char* f);
	friend std::ostream& operator<<(std::ostream& os, const List& bd);
};

std::ostream& operator<<(std::ostream& os, const Node& n);
std::ostream& operator<<(std::ostream& os, const List& bd);
