#pragma once
#include <vector>

#include "date.h"
#include "parsers.h"

struct MoneyState
{
	double earned = 0.0;
	double spent = 0.0;

	double ComputeIncome() const;

    MoneyState& operator+=(const MoneyState& other);
    MoneyState operator+(const MoneyState& other) const;
	MoneyState operator*(double factor) const;
};

struct IndexSegment
{
    size_t left;
    size_t right;

    size_t length() const;
    bool empty() const;
    bool Contains(IndexSegment other) const;
};

static const Date START_DATE = Date::FromString("2000-01-01");
static const Date END_DATE = Date::FromString("2100-01-01");
static const size_t DAY_COUNT = static_cast<size_t>(END_DATE - START_DATE);

class BudgetManager
{
    std::vector<MoneyState> data_;

    size_t getIndex(const Date& date) const;
    IndexSegment MakeDateSegment(const Date& from, const Date& to) const;
public:
    BudgetManager() : data_(DAY_COUNT) {}
    auto MakeDateRange(const Date& from, const Date& to) const {
        const IndexSegment segment = MakeDateSegment(from, to);
        return Range(data_.cbegin() + segment.left, data_.cbegin() + segment.right);
    }
    auto MakeDateRange(const Date& from, const Date& to) {
        const IndexSegment segment = MakeDateSegment(from, to);
        return Range(data_.begin() + segment.left, data_.begin() + segment.right);
    }
};
