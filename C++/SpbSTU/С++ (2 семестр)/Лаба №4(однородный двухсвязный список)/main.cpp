/*
Объектно-ориентированный однородный двухсвязный список.
Цель - с помощью классов реализовать список объектов типа Circle (круг).
Последовательность действий:
	1.	Создайте класс Point
Подумайте: какие методы и переменные должны быть определены в этом классе.
	2.	Создайте класс Circle
Этот класс должен содержать:
внедренный объект типа Point – центр круга
радиус
При конструировании объектов класса Circle предусмотрите эффективную инициализацию внедренных объектов.
	3.	Разработаем двухсвязный список, элементы которого будут являться копиями наших объектов
Замечание: так как в списке будут фигурировать копии объектов – нам все равно, каким образом были созданы объекты-оригиналы
(глобальные, локальные, динамические, автоматические неименованные) => манипулируя списком, мы несем ответственность только за уничтожение копий,
а оригиналы создаются и уничтожаются независимо.
Список реализуется двумя классами:
	3.1.	Вспомогательный класс для хранения данных и связей между объектами.
Замечание: вспомогательный класс можно объявить независимо или «встроить» прямо в объявлении класса, реализующего список. Независимо от реализации удобно,
чтобы все понятия вспомогательного класса были доступны методам класса List => подумайте как можно это обеспечить.
Альтернатива – реализация public методов для доступа к защищенным данным.
	3.2.	Основной класс, реализующий список
Теперь вспомогательный класс Node можно использовать при создании связного списка из объектов типа Circle - создадим еще один класс для реализации списка – List.
	4.	Реализация методов
Реализуйте самостоятельно методы класса List:
1)	метод, который добавляет элемент в начало списка
2)	метод, который добавляет элемент в конец списка
3)	удаление из списка первого элемента, данное которого совпадает со значением параметра. Если элемент найден и удален, метод возвращает true, если элемента с таким значением в списке не было – false
4)	удаление из списка всех элементов, данное которых совпадает со значением параметра. Метод возвращает количество удаленных элементов.
5)	Сделать список пустым
6)	добавьте в класс List остальные, необходимые на Ваш взгляд методы.
	5.	Сортировка для созданного списка и вывод результатов в файл
Напишите для класса List
1.	сортировку по возрастанию площади объекта.
2.	Вывод текущего состояния списка на консоль
	cout<<l;
3.	Вывод текущего состояния списка в файл (в форматированном виде)
#include <fstream>
	ofstream fout(ar);
	fout<<l;
	fout.close();
4.	Чтение из файла
*/
#include "circle.h"
#include "list.h"
#define	  stop __asm nop

int main()
{
	Point p3(6, 3);
	Circle c1(1, 2, 1);
	Circle c2;
	Circle c3(p3, 1);
	Circle c4(2, 4);
	Circle c5(6, 7, 8);
	Circle c6(6, 3, 1);

	List list;
	list.addToTail(c1);
	list.addToTail(c2);
	list.addToTail(c3);
	list.addToHead(c4);
	list.addToHead(c5);
	list.addToHead(c6);

	//отладка
	std::cout << list << std::endl;
	stop
	/*
	1. Point: (6, 3) Radius: 1
	2. Point: (6, 7) Radius: 8
	3. Point: (2, 4) Radius: 0
	4. Point: (1, 2) Radius: 1
	5. Point: (0, 0) Radius: 0
	6. Point: (6, 3) Radius: 1
	*/
	list.sort();
	std::cout << list << std::endl;
	stop
	/*
	1. Point: (2, 4) Radius: 0
	2. Point: (0, 0) Radius: 0
	3. Point: (6, 3) Radius: 1
	4. Point: (1, 2) Radius: 1
	5. Point: (6, 3) Radius: 1
	6. Point: (6, 7) Radius: 8
	*/
	list.writeToFile("file.txt");
	std::cout << list << std::endl;
	stop
	list.readFromFile("file.txt");
	std::cout << list << std::endl; //-> x2 list (12 rows)
	stop

	list.remove(c5); // пробовал х2
	std::cout << list << std::endl; // 11 rows
	stop

	list.removeEvery(c6);
	std::cout << list << std::endl; // 7 rows (c6 + c3) * 2
	stop

	list.clean();
	std::cout << list << std::endl; // 0 rows
	stop
	return 0;
}
