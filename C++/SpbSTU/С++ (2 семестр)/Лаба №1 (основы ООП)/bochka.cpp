#include "bochka.h"
#include <stdio.h>

double amountK = 1; // объем кружки

Bochka::Bochka(double amount, double conc)
{
	if (amount < amountK) // проверка объема
	{
		printf("Wrong value of amount, will set 1");
		this->amount = 1;
	}
	else
	{
		this->amount = amount;
	}
	if (conc > 1) // проверка концентрации
	{
		printf("Wrong value of concentration, will set 0.96");
		this->conc = 0.96;
	}
	else
	{
		this->conc = conc;
	}
}

const double Bochka::getConc() const
{
	return conc;
}

void Bochka::pereliv(Bochka& second)
{
	double C1to2 = (conc * amountK + second.conc * second.amount) / (amountK + second.amount);
	amount -= amountK;
	second.amount += amountK;
	second.conc = C1to2;
}
