#include <iostream>
#include <unordered_set>
using namespace std;

using Int64 = int64_t; // [1; 10^12]
using Int32 = int32_t;

Int64 ComputeNod(Int64 x, Int64 y) {
	while (x > 0 && y > 0) {
		if (x > y) {
			x = x % y;
		} else {
			y = y % x;
		}
	}
	return x + y;
}

Int32 ComputeResult(Int64 NOD, Int64 NOK) {
	Int32 result = 0;

	unordered_set<Int64> delims;
	// 1. Поиск всех делителей [1, sqrt(NOK)]
	for (Int32 i = 1; i <= static_cast<Int32>(sqrt(NOK)); ++i) {
		if (NOK % i == 0) {
			delims.insert(i);
			delims.insert(NOK / i);
		}
	}
	// 2. Поиск всех подходящих пар для NOD(x,y) * NOK(x,y) = x * y
	// но нужна доп. проверка т.к под условие попадают неверные значения
	// NOD = 527, NOK = 9486 -> 1581 3162 -> но для них NOD = 1581, NOK = 3162
	auto NODK = NOD * NOK;
	for (auto itb = delims.begin(), ite = delims.end(); itb != ite; ) {
		auto x = NODK / *itb;
		if (delims.find(x) != ite) { // 
			if (ComputeNod(x, *itb) == NOD) {
				result += 2;
				//cout << x << ' ' << *itb << '\n';
			}
			delims.erase(x);
			itb = delims.erase(itb);
		}
		else {
			++itb;
		}
	}

	return result;
}

int main() {
	Int64 NOD = 0;
	Int64 NOK = 0;
	if (cin >> NOD >> NOK && NOD > 0 && NOK > 0) {
		cout << ComputeResult(NOD, NOK);
	}
	else {
		cout << "Something wrong";
	}
	return 0;
}
