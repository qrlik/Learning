#include "mentor_algorithm.h"
#include <algorithm>

using namespace std;

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

IndexSegment IntersectSegments(IndexSegment lhs, IndexSegment rhs) {
    const size_t left = max(lhs.left, rhs.left);
    const size_t right = min(lhs.right, rhs.right);
    return { left, max(left, right) };
}

bool AreSegmentsIntersected(IndexSegment lhs, IndexSegment rhs) {
    return !(lhs.right <= rhs.left || rhs.right <= lhs.left);
}

// struct MoneyState
double MoneyState::ComputeIncome() const {
    return earned - spend;
}

MoneyState& MoneyState::operator+=(const MoneyState& other) {
    earned += other.earned;
    spend += other.spend;
    return *this;
}

MoneyState MoneyState::operator+(const MoneyState& other) const {
    return MoneyState(*this) += other;
}

MoneyState MoneyState::operator*(double factor) const {
    return { earned * factor, spend * factor };
}

// class BulkLinearUpdater
void BulkLinearUpdater::CombineWith(const BulkLinearUpdater& other) {
    tax_.factor *= other.tax_.factor;
    add_.delta.spend += other.add_.delta.spend;
    add_.delta.earned = add_.delta.earned * other.tax_.factor + other.add_.delta.earned;
}

MoneyState BulkLinearUpdater::Collapse(const MoneyState& origin, IndexSegment segment) const {
    return MoneyState{origin.earned * tax_.factor , origin.spend} + add_.delta * segment.length();
}
