#include "book.h"

const char* Catg[] = { "History", "Fantasy", "Thriller", "Romance" }; // символьный аналог
bool(*cmpAr[]) (const BOOK* const, const BOOK* const) = { cmpName, cmpAuthor, cmpPrice, cmpYear, cmpGenre }; // массив указателей на функции сравнения
void(*menuAr[]) (KARTOTEKA* const) = { printKART, addBOOK, remBOOK, fileOUT, fileIN, sortKART }; // массив указателей на функции. ВОПРОС С const для printKART и fileOUT

void addBOOK(KARTOTEKA* const pkart)
{
	if (pkart->m_n >= pkart->m_cap) // проверка на заполнение
	{
		capKART(pkart);
	}
	pkart->pK[pkart->m_n] = new BOOK; // выделение памяти под новую книгу
	BOOK* pbook = pkart->pK[pkart->m_n]; // оптимизация
	pkart->m_n++;

	printf("Input information about book:\n");

	printf("Name = \n"); // считывание названия
	scanf("%19s", pbook->name);
	fseek(stdin, 0, 0);

	printf("Author = \n"); // считывание автора
	scanf("%19s", pbook->author);
	fseek(stdin, 0, 0);

	double d = 0;
	do
	{
		printf("price = \n"); // считывание цены
		scanf("%lf", &d);
		fseek(stdin, 0, 0);
		if (d < 0)
		{
			printf("Wrong value of price! Please try again\n");
		}
	} while (d < 0);
	pbook->price = d;

	int n = 0;
	do
	{
		printf("year = \n"); // считывание года издания
		scanf("%d", &n);
		fseek(stdin, 0, 0);
		if ((n < 1440) || (n > 2018))
		{
			printf("Wrong value of year! Please try again\n");
		}
	} while ((n < 1440) || (n > 2018));
	pbook->year = n;

	do
	{
		printf("Input genre (History (0), Fantasy (1), Thriller (2), Romance (3)):\n");
		scanf("%d", &n); // считывание категории
		fseek(stdin, 0, 0);
		if ((n < 0) || (n > 3))
		{
			printf("Wrong value of genre! Please try again\n");
		}
	} while ((n < 0) || (n > 3));
	pbook->genre = static_cast<CATEGORY>(n);
	printf("\n");
}

void printKART(KARTOTEKA* const pkart)
{
	for (size_t i = 0; i < pkart->m_n; i++) // печать картотеки
	{
		BOOK* p = pkart->pK[i];
		printf("Name: %s Author: %s Price: %lf Year: %d Category: %s\n", p->name, p->author, p->price, p->year, Catg[p->genre]);
	}
	printf("\n");
}

void remBOOK(KARTOTEKA* const pkart)
{
	int choise;
	do
	{
		printf("1. Delete one book\n2. Delete all books\n3. Not sure, exit\n"); // вывод подменю
		scanf("%d", &choise);
	} while (choise < 1 || choise > 3);

	if (choise == 1)
	{
		printf("What is book number to delete?\n"); // ввод номера книги
		int number;
		do
		{
			printf("Accept only unsigned integer and available number in library\n");
			scanf("%d", &number);
		} while (number <= 0 || number > pkart->m_n);
		delete pkart->pK[number - 1]; // освобождение памяти
		for (size_t i = number - 1; i < pkart->m_n - 1; i++) // сдвиг указателей
		{
			pkart->pK[i] = pkart->pK[i + 1];
		}
		pkart->pK[pkart->m_n] = nullptr; // обнуление последнего указателя
		pkart->m_n--; // уменьшение количества книг на единицу
	}
	else if (choise == 2)
	{
		for (size_t i = 0; i < pkart->m_n; i++) // удаление всех книг
		{
			delete pkart->pK[i];
			pkart->pK[i] = nullptr;
		}
		pkart->m_n = 0; // изменение количества книг на 0
	}
	else
	{
		return;
	}
}

void remKART(KARTOTEKA& pkart)
{
	for (size_t i = 0; i < pkart.m_n; i++) // освобождение памяти
	{
		delete pkart.pK[i];
	}
	delete pkart.pK;
	pkart.pK = nullptr;
}

void capKART(KARTOTEKA* const pkart)
{
	pkart->m_cap += 5; // увеличение книг на пять
	BOOK** tmp = new BOOK*[pkart->m_cap];
	for (size_t i = 0; i < (pkart->m_cap - 5); i++)
	{
		tmp[i] = pkart->pK[i];
	}
	delete[] pkart->pK;
	pkart->pK = tmp;
}

void fileOUT(KARTOTEKA* const pkart)
{
	FILE* file = fopen("my.txt", "w");
	if (file)
	{
		for (size_t i = 0; i < pkart->m_n; i++) // печать данных в файл
		{
			BOOK* pbook = pkart->pK[i];
			fprintf(file, "Name: %s Author: %s Price: %lf Year: %d Category: %d \n", pbook->name, pbook->author, pbook->price, pbook->year, pbook->genre);
		}
	}
	else
	{
		printf("<Opening file ERROR!>");
	}
	fclose(file);
}

void fileIN(KARTOTEKA* const pkart)
{
	FILE* file = fopen("my.txt", "r");
	if (file)
	{
		while (!feof(file))
		{
			if (pkart->m_n >= pkart->m_cap) // проверка на заполнение
			{
				capKART(pkart);
			}
			pkart->pK[pkart->m_n] = new BOOK; // выделение памяти под новую книгу
			BOOK* pbook = pkart->pK[pkart->m_n]; // оптимизация
			fscanf(file, "Name: %s Author: %s Price: %lf Year: %d Category: %d \n", pbook->name, pbook->author, &pbook->price, &pbook->year, &pbook->genre);
			pkart->m_n++;
		}
	}
	else
	{
		printf("<Reading file ERROR!>");
	}
	fclose(file);
}

int Menu()
{
	printf("Enter number of command:\n"
		"1.Print all library\n"
		"2.Add new book\n"
		"3.Delete one or all books\n"
		"4.Write library to file\n"
		"5.Read library from file\n"
		"6.Sort library\n"
		"7.Exit\n\n");
	int choise;
	do
	{
		scanf("%d", &choise);
		if (choise > 0 && choise < 8)
		{
			return choise;
		}
		else
		{
			printf("Wrong number of operation\n");
		}
	} while (true);
}

bool cmpName(const BOOK* const b1, const BOOK* const const b2)
{
	return strcmp(b1->name, b2->name) > 0;
}

bool cmpAuthor(const BOOK* const b1, const BOOK* const b2)
{
	return strcmp(b1->author, b2->author) > 0;
}

bool cmpPrice(const BOOK* const b1, const BOOK* const b2)
{
	return b1->price > b2->price;
}

bool cmpYear(const BOOK* const b1, const BOOK* const b2)
{
	return b1->year > b2->year;
}

bool cmpGenre(const BOOK* const b1, const BOOK* const b2)
{
	return b1->genre > b2->genre;
}

void sortKART(KARTOTEKA* const pkart)
{
	int choise;
	do
	{
		printf("Enter number of field to sort:\n"
		"1.Name\n"
		"2.Author\n"
		"3.Price\n"
		"4.Year\n"
		"5.Genre\n"
		"6.Exit\n");
		scanf("%d", &choise);
	} while (choise < 1 || choise > 6);

	if (choise != 6)
	{
		for (size_t i = 0; i < pkart->m_n - 1; i++)	// пузырьковая сортировка
		{
			bool b = false;
			for (size_t j = 0; j < pkart->m_n - i - 1; j++)
			{
				if (cmpAr[choise - 1](pkart->pK[j], pkart->pK[j + 1]))
				{
					BOOK* tmp = pkart->pK[j];
					pkart->pK[j] = pkart->pK[j + 1];
					pkart->pK[j + 1] = tmp;
					b = true;
				}
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
