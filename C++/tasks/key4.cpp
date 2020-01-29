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

//uint64_t NOK(uint64_t a, uint64_t b)
//{
//	return a * b / NOD(a, b);
//}

bool is_correct(uint64_t a, uint64_t b, uint64_t in_NOD, uint64_t in_NOK) {
	uint64_t a_b_NOD = NOD(a, b);
	return a_b_NOD == in_NOD && (a * b / a_b_NOD == in_NOK);
}

size_t ComputeResult(uint64_t in_NOD, uint64_t in_NOK) {
	if (in_NOD == in_NOK) {
		return 1;
	}

	vector<uint64_t> all_dividers;

	for (uint64_t i = 1; i <= in_NOK; ++i) {
		if (in_NOK % i == 0) {
			all_dividers.push_back(i);
		}
	}

	size_t result = 0;
	for (size_t i = 0; i < all_dividers.size(); ++i) {
		for (size_t j = i + 1; j < all_dividers.size(); ++j) {
			if (is_correct(all_dividers[i], all_dividers[j], in_NOD, in_NOK)) {
				result += 2;
			}
		}
	}
	return result;
}

int main()
{
	uint64_t NOD, NOK;
	cin >> NOD >> NOK;

	cout << ComputeResult(NOD, NOK);
	return 0;
}
