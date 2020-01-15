/*
Напишите функцию, которая обрезает пробелы в конце переданной ей строки.
Функция должна быть написана в расчёте на работу с очень длинными строками с очень большим количеством пробелов, оптимизирована по количеству обращений к памяти.
Сигнатура: void TrimRight( char *s );
*/

#include <string>
#include <cstring>

// на строке из 10^9 пробелов ~1000мс ~850мс(O2 оптимизация)

void TrimRight(char* s) {
	if (!s) {
		return;
	}

	size_t length = strlen(s);
	using step = int;
	step space_mask = *reinterpret_cast<step*>(std::string(sizeof(step), ' ').data());
	step* ptr = reinterpret_cast<step*>(s + length % sizeof(step));

	for (size_t i = length / sizeof(step); i > 0; --i, length -= sizeof(step)) {
		if (ptr[i - 1] != space_mask) {
			break;
		}
	}

	for (; length > 0; --length) { // хвостик
		if (s[length - 1] != ' ') {
			break;
		}
	}

	s[length] = '\0';
}

int main()
{
	return 0;
}
