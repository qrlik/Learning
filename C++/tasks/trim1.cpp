#include <string>
using namespace std;

/*
Напишите функцию, которая обрезает пробелы в конце переданной ей строки.
Функция должна быть написана в расчёте на работу с очень длинными строками с очень большим количеством пробелов, оптимизирована по количеству обращений к памяти.
Сигнатура: void TrimRight( char *s );
*/

// 800мс на строке из 10^9 пробелов

template<typename T, typename Func>
bool IterationBreak(T* ptr, T*& space_suffix, T mask, Func func) { // шаблонная итерация
	if (*ptr == mask) {
		if (!space_suffix) {
			space_suffix = ptr;
		}
	}
	else {
		if (!func(ptr)) {
			space_suffix = nullptr;
		}
		else {
			return true; // ~break
		}
	}
	return false;
}

bool hasZeroByte(const int* ptr) { // проверка на нулевой байт
	const char* pbyte = reinterpret_cast<const char*>(ptr);
	for (size_t i = 0; i < sizeof(int); ++i)
	{
		if (pbyte[i] == 0)
		{
			return true;
		}
	}
	return false;
}

struct Suffix {
	char* ptr;
	bool is_empty; // для случая когда нулевой байт вначале последнего считывания
};

Suffix FindSpaceSuffix(int* ptr) { // поиск суффикса из пробелов
	char* pbyte = reinterpret_cast<char*>(ptr);
	char* pos = nullptr;

	size_t i = 0;
	for (; i < sizeof(int) && *pbyte != 0; ++pbyte, ++i) {
		if (IterationBreak(pbyte, pos, ' ', [](char*) { return false; })) {
			break;
		};
	}

	if (i == 0) {
		return { pos, true };
	}
	return { pos, false };
}

Suffix ComputeLeftSuffix(int* begin, int* space_suffix, int* ptr) {
	Suffix left_suffix = { nullptr, true };
	if (space_suffix) {
		left_suffix = (space_suffix != begin)
			? FindSpaceSuffix(space_suffix - 1)
			: Suffix{ reinterpret_cast<char*>(begin), false };
	}
	else if (ptr != begin) {
		left_suffix = FindSpaceSuffix(ptr - 1);
	}
	return left_suffix;
}

void TrimRight(char* s) {
	if (!s) {
		return;
	}

	int mask = *reinterpret_cast<int*>(string(4, ' ').data()); // маска из пробелов
	int* ptr = reinterpret_cast<int*>(s);
	int* begin = ptr;
	int* space_suffix = nullptr;

	for (; ; ++ptr) {
		if (IterationBreak(ptr, space_suffix, mask, hasZeroByte)) {
			break;
		}
	}
	// условно делим данные на часть слева от ptr и справа (хвостик с нулевым байтом)
	Suffix left_suffix = ComputeLeftSuffix(begin, space_suffix, ptr);
	Suffix right_suffix = FindSpaceSuffix(ptr);

	if (left_suffix.ptr && (right_suffix.ptr == reinterpret_cast<char*>(ptr) || right_suffix.is_empty == true))
	{ // если есть суффикс слева и справа либо нулевой байт либо все пробелы обнуляем левый
		*left_suffix.ptr = 0;
	}
	else if (right_suffix.ptr) {
		*right_suffix.ptr = 0;
	}
}

int main()
{
	return 0;
}
