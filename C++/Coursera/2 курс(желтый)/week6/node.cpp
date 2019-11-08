#include "node.h"

using namespace std;

template <typename T> bool ComparisonData(Comparison cmp, const T& lhs, const T& rhs)
{
	switch (cmp)
	{
	case(Comparison::Less):
		return lhs < rhs;
	case(Comparison::LessOrEqual):
		return lhs <= rhs;
	case(Comparison::Greater):
		return lhs > rhs;
	case(Comparison::GreaterOrEqual):
		return lhs >= rhs;
	case(Comparison::Equal):
		return lhs == rhs;
	case(Comparison::NotEqual):
		return lhs != rhs;
	default:
		throw invalid_argument("Unknown Comparison type");
	}
}

// реализация EmptyNode
bool EmptyNode::Evaluate(const Date& date, const string& event) const
{ // подходят все элементы. я бы лучше убрал этот класс и просто возвращал nullptr. и если Nullptr вызывал бы FindAll | DeleteAll
	return true;
}

// реализация DateComparisonNode
DateComparisonNode::DateComparisonNode(Comparison cmp, const Date& date) : _cmp(cmp), _date(date) {};

bool DateComparisonNode::Evaluate(const Date& date, const string& event) const
{
	return ComparisonData(_cmp, date, _date);
}

// реализация EventComparisonNode
EventComparisonNode::EventComparisonNode(Comparison cmp, const string& event) : _cmp(cmp), _event(event) {};

bool EventComparisonNode::Evaluate(const Date& date, const string& event) const
{
	return ComparisonData(_cmp, event, _event);
}

// реализация LogicalOperationNode
LogicalOperationNode::LogicalOperationNode(LogicalOperation operation, shared_ptr<Node> lhs, shared_ptr<Node> rhs) :
	_operation(operation), _lhs(lhs), _rhs(rhs) {};

bool LogicalOperationNode::Evaluate(const Date& date, const string& event) const
{
	switch (_operation)
	{
	case(LogicalOperation::And):
		return _lhs->Evaluate(date, event) && _rhs->Evaluate(date, event);
	case(LogicalOperation::Or):
		return _lhs->Evaluate(date, event) || _rhs->Evaluate(date, event);
	default:
		throw invalid_argument("Unknown LogicalOperation type");
	}
}

