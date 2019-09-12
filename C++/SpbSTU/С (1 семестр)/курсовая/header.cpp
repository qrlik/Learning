#include "rectangle.h"

namespace { size_t count = 0; }
bool(*cmpList[]) (const rectangle* const, const rectangle* const) = { cmpA, cmpB, cmpS }; // массив указателей на функции сравнения
void(*menuRec[]) (list* const) = { addRectangle, printLIST, remRectangle, remSquare, fileOUT, fileIN, sortLIST }; // массив указателей на функции меню

void printLIST(list* const list)
{
	rectangle* tmp = list->head;
	for (size_t i = 1; i <= count; i++)
	{
		printf("%2d. Length: %.2lf Width: %.2lf Square: %.4lf\n", i, tmp->a, tmp->b, tmp->S);
		tmp = tmp->next;
	}
	printf("\n");
}

void deleteRectangle(rectangle* const tmp, list* const list)
{
	if (tmp->prev != nullptr) // если это не первый элемент
	{
		tmp->prev->next = tmp->next; // направляем next предыдущего элемента на последующий
	}
	else // если это первый элемент
	{
		list->head = tmp->next; // направляем head списка на следующий
	}
	if (tmp->next != nullptr) // если это не последний элемент
	{
		tmp->next->prev = tmp->prev; // направляем prev последующего на предыдущий
	}
	else // если это последний элемент
	{
		list->tail = tmp->prev; // напряевляем tail списка на предыдущий
	}
	delete tmp; // освобождение памяти
	count--; // уменьшение счетчика элементов
}

void addRectangle(list* const list)
{
	rectangle* tmp = new rectangle;
	tmp->next = nullptr;

	printf("Input information about rectangle:\n");
	double d = 0;
	do
	{
		printf("Length = \n"); // считывание длины
		scanf("%lf", &d); // .2lf НИКАК НЕ СДЕЛАТЬ????????????? округление там или еще что
		fseek(stdin, 0, 0);
		if (d <= 0)
		{
			printf("Wrong value of length! Please try again\n");
		}
	} while (d <= 0);
	tmp->a = d;
	do
	{
		printf("Width = \n"); // считывание ширины
		scanf("%lf", &d);
		fseek(stdin, 0, 0);
		if (d <= 0)
		{
			printf("Wrong value of width! Please try again\n");
		}
	} while (d <= 0);
	tmp->b = d;

	tmp->S = tmp->a * tmp->b; // рассчет площади

	if (list->head != nullptr)
	{
		tmp->prev = list->tail;
		list->tail->next = tmp;
		list->tail = tmp;
	}
	else
	{
		tmp->prev = nullptr;
		list->head = list->tail = tmp;
	}
	count++; // увеличение счетчика прямоугольников
}

void remRectangle(list* const list)
{
	int choise;
	do
	{
		printf("1. Delete one rectangle\n2. Delete all rectangles\n3. Not sure, exit\n"); // вывод подменю
		scanf("%d", &choise);
	} while (choise < 1 || choise > 3);

	if (choise == 1) // удаление 1 элемента
	{
		printf("What is rectangle number to delete?\n"); // ввод номера книги
		int number;
		do
		{
			printf("Accept only unsigned integer and available number in list\n");
			scanf("%d", &number);
		} while (number <= 0 || number > count);

		rectangle* tmp = list->head;
		for (size_t i = 1; i < number; i++) // поиск нужного элемента. например, для 3 элемента нужно сделать 2 перехода от головы, поэтому i = 1
		{
			tmp = tmp->next;
		}
		// стоит ли оптимизировать поиск с выбором его начала (head,tail)?
		deleteRectangle(tmp, list); // функция удаления 1 узла
	}
	else if (choise == 2) // удаление всех элементов
	{
		// у меня remList(&)
		rectangle* tmp = nullptr;
		while (list->head)
		{
			tmp = list->head->next;
			delete list->head;
			list->head = tmp;
		}
		list->tail = nullptr;
		count = 0; // обнуление счетчика
	}
}

void remSquare(list* const list)
{
	printf("Enter value of square with which delete rectangles\n");
	double e = 0.001; // точность
	double square;
	do
	{
		printf("Square = \n"); // считывание площади
		scanf("%lf", &square);
		fseek(stdin, 0, 0);
		if (square <= 0)
		{
			printf("Wrong value of square! Please try again\n");
		}
	} while (square <= 0);

	rectangle* tmp = list->head;
	while (tmp)
	{
		if ((tmp->S - square) < e && (tmp->S - square) > -e) // если площадь равна с учетом точности
		{
			rectangle* next = tmp->next; // создание локального указателя на следующий элемент
			deleteRectangle(tmp, list);
			tmp = next;
		}
		else
		{
			tmp = tmp->next;
		}
		// что лучше +1 переменная с 2 определениями или писать list->head->S ?????????????????????????????
	}
}

bool cmpA(const rectangle* const b1, const rectangle* const b2)
{
	return b1->a > b2->a;
}

bool cmpB(const rectangle* const b1, const rectangle* const b2)
{
	return b1->b > b2->b;
}

bool cmpS(const rectangle* const b1, const rectangle* const b2)
{
	return b1->S > b2->S;
}

void sortLIST(list* const list)
{
	int choise;
	do
	{
		printf("Enter number of field to sort:\n"
			"1.Length\n"
			"2.Width\n"
			"3.Square\n"
			"4.Exit\n");
		scanf("%d", &choise);
	} while (choise < 1 || choise > 4);

	if (choise != 4)
	{
		for (size_t i = 0; i < count - 1; i++)	// пузырьковая сортировка
		{
			bool b = false;
			rectangle* tmp = list->head;
			rectangle* next = tmp->next;
			rectangle* prev = tmp->prev;
			for (size_t j = 0; j < count - i - 1; j++)
			{
				if (cmpList[choise - 1](tmp, next)) // сортировка в зависимости от выбранного поля
				{
					if (prev != nullptr) // если tmp не в начале списка
					{
						prev->next = tmp->next; // next
						next->prev = tmp->prev; // prev
					}
					else // если мы в самом начале
					{
						next->prev = nullptr;
						list->head = next;
					}
					tmp->next = next->next; // перенаправление указателей в соответствии с сортировкой
					tmp->prev = next;
					next->next = tmp;
					if (tmp->next != nullptr) // проверка на перестановку в хвосте
					{
						tmp->next->prev = tmp;
					}
					else
					{
						list->tail = tmp;
					}

					b = true;
					prev = next;
				}
				else
				{
					prev = tmp;
					tmp = next; 
				}
				next = tmp->next;
			}
			if (b == false)
			{
				break;
			}
		}
	}
	else
	{
		return;
	}
}

void fileOUT(list* const list)
{
	FILE* file = fopen("my.txt", "w");
	if (file)
	{
		rectangle* tmp = list->head;
		while(tmp) // печать данных в файл
		{
			fprintf(file, "Length: %lf Width: %lf Square: %lf \n", tmp->a, tmp->b, tmp->S);
			tmp = tmp->next;
		}
	}
	else
	{
		printf("<Opening file ERROR!>");
	}
	fclose(file);
}

void fileIN(list* const list)
{
	FILE* file = fopen("my.txt", "r");
	if (file)
	{
		rectangle* tmp;
		while (!feof(file))
		{
			tmp = new rectangle; // выделение памяти
			tmp->next = nullptr;
			
			fscanf(file, "Length: %lf Width: %lf Square: %lf \n", &tmp->a, &tmp->b, &tmp->S);
			if (list->head != nullptr)
			{
				tmp->prev = list->tail;
				list->tail->next = tmp;
				list->tail = tmp;
			}
			else
			{
				tmp->prev = nullptr;
				list->head = list->tail = tmp;
			}
			count++; // увеличение счетчика прямоугольников
		}
	}
	else
	{
		printf("<Reading file ERROR!>");
	}
	fclose(file);
}

void remList(list& list)
{
	rectangle* tmp = nullptr;
	while (list.head)
	{
		tmp = list.head->next;
		delete list.head;
		list.head = tmp;
	}
	list.tail = nullptr;
}

int Menu()
{
	printf("\nEnter number of command:\n"
		"1.Add new rectangle\n"
		"2.Print all list\n"
		"3.Delete one or all rectangles\n"
		"4.Delete rectangles only with certain square\n"
		"5.Write list to file\n"
		"6.Read list from file\n"
		"7.Sort list\n"
		"8.Exit\n\n");
	int choise;
	do
	{
		scanf("%d", &choise);
		fseek(stdin, 0, 0);
		if (choise > 0 && choise < 9)
		{
			return choise;
		}
		else
		{
			printf("Wrong number of operation. Enter again\n");
		}
	} while (true);
}

