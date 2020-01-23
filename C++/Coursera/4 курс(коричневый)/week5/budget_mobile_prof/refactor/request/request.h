#pragma once
#include <vector>

#include "date.h"
#include "parsers.h"
#include "mentor_algorithm.h"

static const Date START_DATE = Date::FromString("2000-01-01");
static const Date END_DATE = Date::FromString("2100-01-01");
static const size_t DAY_COUNT = ComputeDaysDiff(END_DATE, START_DATE);

size_t ComputeDayIndex(const Date& date);
IndexSegment MakeDateSegment(const Date& date_from, const Date& date_to);

class BudgetManager : public SummingSegmentTree<MoneyState, BulkLinearUpdater> {
public:
	BudgetManager() : SummingSegmentTree(DAY_COUNT) {}
};

