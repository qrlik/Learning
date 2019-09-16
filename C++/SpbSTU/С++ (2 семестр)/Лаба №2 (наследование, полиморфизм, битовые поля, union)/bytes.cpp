#include "bytes.h"
#include <iostream>

using namespace std;

//Bin - для двоичного представления
void Bin::Show() const
{
	cout << '\n' << m_bin7 << m_bin6 << m_bin5 << m_bin4 << m_bin3 << m_bin2 << m_bin1 << m_bin0 << endl;
}

void Bin::GetPos(int& pos) const
{
	pos = (pos >= 1 && pos <= 8) ? pos - 1 : 0;
	bool arr[8] = { m_bin0, m_bin1, m_bin2, m_bin3, m_bin4, m_bin5, m_bin6, m_bin7 }; // можно через switch
	cout << '\n' << arr[pos] << endl;
}

void Bin::SetPos(int& pos, int& val)
{
	pos = (pos >= 1 && pos <= 8) ? pos - 1 : 0;
	val = (val) ? 1 : 0;
	switch (pos)
	{
		case (0):
			m_bin0 = val;
			break;
		case (1):
			m_bin1 = val;
			break;
		case (2):
			m_bin2 = val;
			break;
		case (3):
			m_bin3 = val;
			break;
		case (4):
			m_bin4 = val;
			break;
		case (5):
			m_bin5 = val;
			break;
		case (6):
			m_bin6 = val;
			break;
		case (7):
			m_bin7 = val;
			break;
	}
}

//Oct - для восьмеричного представления
void Oct::Show() const
{
	cout << '\n' << m_oct2 << m_oct1 << m_oct0 << endl;
}

void Oct::GetPos(int& pos) const
{
	pos = (pos >= 1 && pos <= 3) ? pos - 1 : 0;
	unsigned char arr[] = { m_oct0, m_oct1, m_oct2};
	cout << '\n' << arr[pos] << endl;
}

void Oct::SetPos(int& pos, int& val)
{
	pos = (pos >= 1 && pos <= 3) ? pos - 1 : 0;
	val = (val >= 0 && val <= 7) ? val : 0;
	switch (pos)
	{
	case (0):
		m_oct0 = val;
		break;
	case (1):
		m_oct1 = val;
		break;
	case (2):
		m_oct2 = val;
		break;
	}
}

//Hex - для шестнадцатерчного представления
const char* Hex::str = "0123456789abcdef";

void Hex::Show() const
{
	cout << '\n' << str[m_hex1] << str[m_hex0] << endl;
}

void Hex::GetPos(int& pos) const
{
	switch (pos)
	{
	case 0:
		cout << '\n' << str[m_hex0] << endl;
		break;
	case 1:
		cout << '\n' << str[m_hex1] << endl;
		break;
	default:
		cout << "Wrong position!" << endl;
	}
}

void Hex::SetPos(int& pos, int& val)
{
	if ((val < 0) || (val > 15))
	{
		cout << "Wrong value!" << endl;
		return;
	}

	switch (pos)
	{
	case 0:
		m_hex0 = val;
		break;
	case 1:
		m_hex1 = val;
		break;
	default:
		cout << "Wrong position!" << endl;
	}
}

void Bytes::ShowDec() const
{
	cout << '\n' << static_cast<unsigned int>(m_dec) << endl;
}

void Bytes::ShowChar() const
{
	cout << '\n' << m_dec << endl;
}
