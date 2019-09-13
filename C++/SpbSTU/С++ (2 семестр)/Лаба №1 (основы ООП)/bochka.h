#pragma once

class Bochka
{
	double amount;
	double conc;

public:
	Bochka(double amount = 1.0, double conc = 0.96);
	const double getConc() const;
	void pereliv(Bochka& from);
};
