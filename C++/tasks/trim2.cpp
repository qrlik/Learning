#include <string>
#include <cstring>
using namespace std;

/*
Напишите функцию, которая обрезает пробелы в конце переданной ей строки.
Функция должна быть написана в расчёте на работу с очень длинными строками с очень большим количеством пробелов, оптимизирована по количеству обращений к памяти.
Сигнатура: void TrimRight( char *s );
*/

// ~880мс на строке из 10^9 пробелов

void TrimRight(char* s) {
	if (!s) {
		return;
	}

	size_t len = strlen(s);
	int mask = *reinterpret_cast<int*>(string(4, ' ').data());
	int* new_ptr = reinterpret_cast<int*>(s + len % 4);

	for (size_t i = len / 4; i > 0; --i, len -= 4) {
		if (new_ptr[i - 1] != mask) {
			break;
		}
	}

	for (; len > 0; --len) {
		if (s[len - 1] != ' ') {
			break;
		}
	}

	s[len] = '\0';
}

int main()
{
	return 0;
}
