#pragma once
#include <array>

#include "date.h"

static const Date START_DATE = Date::FromString("2000-01-01");
static const Date END_DATE = Date::FromString("2100-01-01");
static const size_t DAY_COUNT = ComputeDaysDiff(END_DATE, START_DATE);
static const size_t DAY_COUNT_P2 = 1 << 16;
static const size_t VERTEX_COUNT = DAY_COUNT_P2 * 2;

size_t ComputeDayIndex(const Date& date) {
    return ComputeDaysDiff(date, START_DATE);
}

struct MoneyState {
    double earned = 0.0;
    double spend = 0.0;

    double ComputeIncome() const {
        return earned - spend;
    }
};

MoneyState operator+(const MoneyState& lhs, const MoneyState& rhs) {
    return { lhs.earned + rhs.earned, lhs.spend + rhs.spend };
}

std::array<MoneyState, VERTEX_COUNT> tree_values, tree_add;
std::array<double, VERTEX_COUNT> tree_factor;

void Init() {
    tree_values.fill(MoneyState());
    tree_add.fill(MoneyState());
    tree_factor.fill(1.0);
}

void Push(size_t v, size_t l, size_t r) {
    for (size_t w = v * 2; w <= v * 2 + 1; ++w) {
        if (w < VERTEX_COUNT) {
            tree_factor[w] *= tree_factor[v];
            tree_add[w].earned *= tree_factor[v];
            tree_add[w].earned += tree_add[v].earned;
            tree_add[w].spend += tree_add[v].spend;
            tree_values[w].earned *= tree_factor[v];
            tree_values[w].earned += tree_add[v].earned * (r - l) / 2;
            tree_values[w].spend += tree_add[v].spend * (r - l) / 2;
        }
    }
    tree_factor[v] = 1.0;
    tree_add[v] = MoneyState();
}

double ComputeSum(size_t v, size_t l, size_t r, size_t from, size_t to) {
    if (v >= VERTEX_COUNT || to <= l || r <= from) {
        return 0;
    }
    Push(v, l, r);
    if (from <= l && r <= to) {
        return tree_values[v].ComputeIncome();
    }
    return ComputeSum(v * 2, l, (l + r) / 2, from, to)
        + ComputeSum(v * 2 + 1, (l + r) / 2, r, from, to);
}

void Add(size_t v, size_t l, size_t r, size_t ql, size_t qr, MoneyState value) {
    if (v >= VERTEX_COUNT || qr <= l || r <= ql) {
        return;
    }
    Push(v, l, r);
    if (ql <= l && r <= qr) {
        tree_add[v].earned += value.earned;
        tree_add[v].spend += value.spend;
        tree_values[v].earned += value.earned * (r - l);
        tree_values[v].spend += value.spend * (r - l);
        return;
    }
    Add(v * 2, l, (l + r) / 2, ql, qr, value);
    Add(v * 2 + 1, (l + r) / 2, r, ql, qr, value);
    tree_values[v] =
        (v * 2 < VERTEX_COUNT ? tree_values[v * 2] : MoneyState())
        + (v * 2 + 1 < VERTEX_COUNT ? tree_values[v * 2 + 1] : MoneyState());
}

void Multiply(size_t v, size_t l, size_t r, size_t ql, size_t qr, double factor) {
    if (v >= VERTEX_COUNT || qr <= l || r <= ql) {
        return;
    }
    Push(v, l, r);
    if (ql <= l && r <= qr) {
        tree_factor[v] *= factor;
        tree_add[v].earned *= factor;
        tree_values[v].earned *= factor;
        return;
    }
    Multiply(v * 2, l, (l + r) / 2, ql, qr, factor);
    Multiply(v * 2 + 1, (l + r) / 2, r, ql, qr, factor);
    tree_values[v] =
        (v * 2 < VERTEX_COUNT ? tree_values[v * 2] : MoneyState())
        + (v * 2 + 1 < VERTEX_COUNT ? tree_values[v * 2 + 1] : MoneyState());
}
