#include <cstdint>
#include <vector>
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

vector<pair<uint64_t, uint64_t>> findAllPairsFromNOK(uint64_t value) {
    vector<uint64_t> all_dividers;

    for (uint64_t i = 1; i <= value; ++i) {
        if (value % i == 0) {
            all_dividers.push_back(i);
        }
    }

	vector<pair<uint64_t, uint64_t>> all_pairs;
	for (size_t i = 0; i < all_dividers.size(); ++i) {
		for (size_t j = i; j < all_dividers.size(); ++j) {
			if (NOK(all_dividers[i], all_dividers[j]) == value) {
				all_pairs.push_back({ all_dividers[i], all_dividers[j] });
			}
		}
	}
	return all_pairs;
}

size_t findIntersectionFromNOD(uint64_t value, const vector<pair<uint64_t, uint64_t>>& pairs) {
	size_t result = 0;

	for (size_t i = 0; i < pairs.size(); ++i) {
		if (NOD(pairs[i].first, pairs[i].second) == value) {
			result += (pairs[i].first == pairs[i].second) ? 1 : 2;
		}
	}

	return result;
}

int main()
{
	uint64_t NOD, NOK;
	cin >> NOD >> NOK;
	vector<pair<uint64_t, uint64_t>> pair_from_NOK = findAllPairsFromNOK(NOK);
	size_t result = findIntersectionFromNOD(NOD, pair_from_NOK);
	cout << result;
    return 0;
}
