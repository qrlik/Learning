#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

// тип для значений лежаков 0 <= t <= 10^9
using SBType = int32_t;
// тип для количества лежаков 2 <= n <= 10^6
using SBAmType = int32_t;
// максимальное количество лежаков
const SBAmType MAX_AMOUNT = 1'000'000;

template <typename Number>
Number ReadNumber(std::istream& stream) {
	Number number;
	stream >> number;
	stream.ignore(numeric_limits<streamsize>::max(), '\n');
	return number;
}

int main() {
	auto tests_amount = ReadNumber<int16_t>(cin);
	vector<SBType> answers;
	answers.reserve(tests_amount);

	vector<SBType> sunbeds(MAX_AMOUNT);
	for (decltype(tests_amount) test = 0; test < tests_amount; ++test) {
		auto sunbeds_amount = ReadNumber<SBAmType>(cin);
		SBType tmp{};

		for (SBAmType i = 0; i < sunbeds_amount; ++i) {
			cin >> tmp;
			sunbeds[i] = tmp;
		}
		sort(sunbeds.begin(), next(sunbeds.begin(),sunbeds_amount));

		SBType answer = sunbeds[0] ^ sunbeds[1]; // sunbeds_amount (2 <= n <= 10^6)
		for (SBAmType i = 1; i + 1 < sunbeds_amount; ++i) {
			answer = min(answer, sunbeds[i] ^ sunbeds[i + 1]);
		}
		answers.push_back(answer);
	}

	for (auto i : answers) {
		cout << i << '\n';
	}
	return 0;
}
