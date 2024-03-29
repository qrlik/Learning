/*
	Статические члены класса
	Задание: требуется реализовать класс MyString таким образом, чтобы все строки хранились в единственном экземпляре. Это можно сделать посредством односвязного списка
Вспомогательный класс Counter является "оберткой" для строки и содержит вспомогательные данные, которые в частности позволят избежать дублирования данных.
Переменные (обычные):
•	указатель на динамически создаваемую строку – m_pStr
•	счетчик пользователей данной строки - m_nOwners
•	указатель на следующий Counter – pNext
Методы:
•	конструкторы
•	деструктор
•	добавить пользователя
•	убрать пользователя 
	Для того, чтобы обеспечить хранение строки в единственном экземпляре, в классе Counter должен храниться в единственном экземпляре перечень всех вспомогательных оберток,
(таким образом, в любой момент выполнения программы можно получить перечень всех используемых строк). Введем статические данные для организации списка:
static Counter* Head;
	А также счетчик указателей:
static unsigned int m_curCounters;
Следует предусмотреть определение и инициализацию статических данных.
	Основной класс MyString
содержит единственное данное – указатель на "свой" Counter –
Counter* m_pMyCounter
Введите необходимые на Ваш взгляд методы («джентльменский набор» для сложного класса)
Для манипуляции всеми строками введите в класс статические методы:
•	распечатать все строки
•	во всех объектах MyString изменить регистр
•	вывести строки по алфавиту
*/
#include "MyString.h"
#define	  stop __asm nop

int main()
{
	//отладка
	MyString s1("ABC!");
	MyString::print();
	stop // s1 - owners 1
	{
		MyString s2("ABC!");
		MyString s3;
		stop // s1,s2 - owners 2, s3 - owners 1 ("empty")
		s3 = s1;
		MyString::print();
		stop // owners 3
	} // ~s2 ~s3
	MyString s2("0Testing");
	stop //s1 - owners 1 ; // s2 - owners 1
	MyString s3(s2);
	MyString s4("Finishing!");
	MyString s5("Ok$");
	MyString::print();
	stop
	/*
	1. Ok$
	2. Finishing!
	3. 0Testing
	4. ABC!
	*/
	MyString::changeReg();
	MyString::print();
	stop
	/*
	1. oK$
	2. fINISHING!
	3. 0tESTING
	4. abc!
	*/
	MyString::sort();
	MyString::print();
	stop
	/*
	1. 0tESTING
	2. abc!
	3. fINISHING!
	4. oK$
	*/
	return 0;
}
