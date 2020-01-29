#include <cstdint>
#include <vector>
#include <unordered_set>
#include <iostream>
using namespace std;

uint64_t NOD(uint64_t a, uint64_t b)
{
	while (a > 0 && b > 0) {
		if (a > b) {
			a %= b;
		}
		else {
			b %= a;
		}
	}
	return a + b;
}

uint64_t NOK(uint64_t a, uint64_t b)
{
	return a * b / NOD(a, b);
}

using Pair = pair<uint64_t, uint64_t>;
struct PairHasher {
	uint64_t operator()(const Pair& p) const {
		return p.first * 37 + p.second;
	}
	hash<uint64_t> ihash;
};

struct PairCompare {
	bool operator()(const Pair& lhs, const Pair& rhs) const {
		return (lhs.first == rhs.first && lhs.second == rhs.second) ||
			(lhs.first == rhs.second && lhs.second == rhs.first);
	}
};

using PairSet = unordered_set<Pair, PairHasher, PairCompare>;

auto findAllPairsFromNOK(uint64_t value) {
	vector<uint64_t> all_dividers;

	for (uint64_t i = 1; i <= value; ++i) {
		if (value % i == 0) {
			all_dividers.push_back(i);
		}
	}

	PairSet all_pairs;
	for (size_t i = 0; i < all_dividers.size(); ++i) {
		for (size_t j = i; j < all_dividers.size(); ++j) {
			if (NOK(all_dividers[i], all_dividers[j]) == value) {
				all_pairs.insert({ all_dividers[i], all_dividers[j] });
			}
		}
	}
	return all_pairs;
}

size_t findIntersectionFromNOD(uint64_t value, const PairSet& pairs) {
	size_t result = 0;

	for (auto pair : pairs) {
		if (NOD(pair.first, pair.second) == value) {
			result += (pair.first == pair.second) ? 1 : 2;
		}
	}

	return result;
}

int main()
{
	uint64_t NOD, NOK;
	cin >> NOD >> NOK;
	auto pair_from_NOK = findAllPairsFromNOK(NOK);
	size_t result = findIntersectionFromNOD(NOD, pair_from_NOK);
	cout << result;
	return 0;
}
