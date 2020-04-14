#include "functions.h"

using namespace std;

shared_ptr<human> human::child(human&& new_human,
	shared_ptr<human> f_parent, shared_ptr<human> s_parent) {
	if (f_parent == s_parent) {
		s_parent = nullptr;
	}
	auto result = make_shared<human>(move(new_human));
	if (f_parent) {
		f_parent->_childs.push_back(result);
		result->_generation = f_parent->_generation;
	}
	if (s_parent) {
		s_parent->_childs.push_back(result);
		result->_generation = max(result->_generation, s_parent->_generation);
	}
	result->_parents.first = std::move(f_parent);
	result->_parents.second = std::move(s_parent);
	++result->_generation;
	return result;
}

void human::print_level(ostream& output, size_t level) const {
	if (_parents.first) {
		_parents.first->print_level(output, level - 1);
	}
	output << string(level, '\t') << _name << '\n';
	if (_parents.second) {
		_parents.second->print_level(output, level - 1);
	}
}

void human::print(ostream& output) const {
	print_level(output, _generation);
}
