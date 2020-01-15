#include "budget.h"

using namespace std;

// struct MoneyState
double MoneyState::ComputeIncome() const {
	return earned - spent;
}

MoneyState& MoneyState::operator+=(const MoneyState& other) {
    earned += other.earned;
    spent += other.spent;
    return *this;
}

MoneyState MoneyState::operator+(const MoneyState& other) const {
    return MoneyState(*this) += other;
}

MoneyState MoneyState::operator*(double factor) const {
    return { earned * factor, spent * factor };
}

// struct IndexSegment
size_t IndexSegment::length() const {
    return right - left;
}
bool IndexSegment::empty() const {
    return length() == 0;
}

bool IndexSegment::Contains(IndexSegment other) const {
    return left <= other.left && other.right <= right;
}

size_t BudgetManager::getIndex(const Date& date) const
{ // date должна быть больше 2000-01-01 по условию задачи
    int64_t result = date - START_DATE;
    if (result < 0 || result >= static_cast<int64_t>(data_.size()))
    {
        ostringstream error;
        error << date.ToString() << " is incorrect for START_DATE = " << START_DATE.ToString();
        throw invalid_argument(error.str());
    }
    return static_cast<size_t>(result);
}

IndexSegment BudgetManager::MakeDateSegment(const Date& from, const Date& to) const
{
    return { getIndex(from), getIndex(to) + 1 }; // [from, to]
}
