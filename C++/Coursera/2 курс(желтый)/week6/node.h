#pragma once

#include "date.h"
#include <memory>

enum LogicalOperation { Or, And };
enum Comparison
{
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual
};

/*
						NODE
			/			 |				\
	EmptyNode	ComparisonNode<T>	LogicalOperationNode
*/

class Node // класс ограничений
{
public:
	virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
};

class EmptyNode : public Node // пустое ограничение
{
public:
	virtual bool Evaluate(const Date& date, const std::string& event) const override;
};

/* Следующие 2 класса лучше заменить шаблонным классом */
class DateComparisonNode : public Node // условие для дат
{
	const Comparison _cmp;
	const Date _date;
public:
	DateComparisonNode(Comparison cmp, const Date& date);
	virtual bool Evaluate(const Date& date, const std::string& event) const override;
};

class EventComparisonNode : public Node // условие для событий
{
	const Comparison _cmp;
	const std::string _event;
public:
	EventComparisonNode(Comparison cmp, const std::string& event);
	virtual bool Evaluate(const Date& date, const std::string& event) const override;
};

class LogicalOperationNode : public Node // ограничение с логикой
{
	const LogicalOperation _operation;
	const std::shared_ptr<Node> _lhs;
	const std::shared_ptr<Node> _rhs;
public:
	LogicalOperationNode(LogicalOperation operation, std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs);
	virtual bool Evaluate(const Date& date, const std::string& event) const override;
};
