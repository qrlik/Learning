#pragma once

//Bin - для двоичного представления
class Bin
{
	bool m_bin0 : 1; // 8 бит для двоичного 11 111 111 - 0xff
	bool m_bin1 : 1;
	bool m_bin2 : 1;
	bool m_bin3 : 1;
	bool m_bin4 : 1;
	bool m_bin5 : 1;
	bool m_bin6 : 1;
	bool m_bin7 : 1;
public:
	void Show() const; //выводим все двоичные цифры
	void GetPos(int& pos) const; //выводим цифру в позиции
	void SetPos(int& pos, int& val);  //изменяем цифру в позиции
	friend union Bytes;
};
//Oct - для восьмеричного представления
class Oct
{
	bool m_oct0 : 3; // 8 бит для 8-ричного [0-3][0-7][0-7] - 0xff
	bool m_oct1 : 3;
	bool m_oct2 : 2;
public:
	void Show() const; //выводим все восьмеричные цифры
	void GetPos(int& pos) const; //выводим цифру в позиции
	void SetPos(int& pos, int& val);  //изменяем цифру в позиции
	friend union Bytes;
};
//Hex - для шестнадцатерчного представления
class Hex
{
	static const char* str;
	bool m_hex0 : 4; // 8 бит для 16-ричного - 0xff
	bool m_hex1 : 4;
public:
	void Show() const; //выводим все шестнадцатиричные цифры
	void GetPos(int& pos) const; //выводим цифру в позиции
	void SetPos(int& pos, int& val);  //изменяем цифру в позиции
	friend union Bytes;
};

union Bytes // union позволяет один байт рассматривать разными группами битов
{
		Bin m_bin;
		Oct m_oct;
		Hex m_hex;
		unsigned char m_dec;
	public:
	Bytes(const unsigned char byte) :m_dec(byte) {}; //конструктор
	void ShowBin() { m_bin.Show(); }
	void ShowOct() { m_oct.Show(); }
	void ShowHex() { m_hex.Show(); }
	void ShowDec() const;
	void ShowChar() const;
	void ShowBinPos(int& pos) { m_bin.GetPos(pos); }
	void ShowOctPos(int& pos) { m_oct.GetPos(pos); }
	void ShowHexPos(int& pos) { m_hex.GetPos(pos); }
	void EditBin(int& pos, int& val) { m_bin.SetPos(pos, val); }
	void EditOct(int& pos, int& val) { m_oct.SetPos(pos, val); }
	void EditHex(int& pos, int& val) { m_hex.SetPos(pos, val); }
};
