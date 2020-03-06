#include "functions.h"

using namespace std;

template<>
void Negate<string>(string& str) {
	transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return static_cast<char>(isupper(c) ? tolower(c) : toupper(c)); });
}

const unordered_map<string, COLORS> EnumMap<COLORS>::str_to_enum = { {"Blue", COLORS::BLUE}, {"Red", COLORS::RED}, {"Green", COLORS::GREEN} };
