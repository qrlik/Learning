#include <string>
#include <cstring>
using namespace std;

/*
Напишите функцию, которая обрезает пробелы в конце переданной ей строки.
Функция должна быть написана в расчёте на работу с очень длинными строками с очень большим количеством пробелов, оптимизирована по количеству обращений к памяти.
Сигнатура: void TrimRight( char *s );
*/

// ~1500мс на строке из 10^9 пробелов

void TrimRight(char* s) {
	if (!s) {
		return;
	}

	char* trim_ptr = s;
	for (; *s != '\0'; ++s) {
		if (*s != ' ') {
			trim_ptr = s + 1;
		}
	}

	*trim_ptr = '\0';
}

int main()
{
	return 0;
}
