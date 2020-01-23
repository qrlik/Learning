#include "budget.h"

using namespace std;

// class BudgetManager
size_t ComputeDayIndex(const Date& date) {
    return ComputeDaysDiff(date, START_DATE);
}

IndexSegment MakeDateSegment(const Date& date_from, const Date& date_to) {
    return { ComputeDayIndex(date_from), ComputeDayIndex(date_to) + 1 };
}
