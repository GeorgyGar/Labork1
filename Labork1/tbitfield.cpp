// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
/* ext 
int  BitLen; // длина битового поля - макс. к-во битов // количество бит в передаваемых байтах
TELEM *pMem; // память для представления битового поля
int  MemLen; // к-во эл-тов Мем для представления бит.поля */
TBitField::TBitField(int len)
{
	if (len < 1)
	{
		throw(len);
	}
	BitLen = len;
	MemLen = (len + 31) >>5;
	pMem = new TELEM[MemLen];
	if (pMem != 0)
		for (register int i = 0; i < MemLen; i++)
			pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != 0)
		for (register int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete []pMem;
	pMem = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n >> 5);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (1 << (n & 31));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n<0) || (n>BitLen))
	{
		throw(n);
	}

		pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n<0) || (n>BitLen))
	{
		throw(n);
	}
	pMem[GetMemIndex(n)] &= ~GetMemMask(n); // U
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n<0) || (n>BitLen))
	{
		throw(n);
	}
		return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	if (pMem != 0)
		for (register int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	unsigned int res = 1; //заранее равны
	if (MemLen != bf.MemLen) return 0; //не равны
	for (register int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
		{
			res = 0;
			break;
		}
	return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (*this == bf)
		return 0; else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (bf.BitLen > BitLen) len = bf.BitLen;
	TBitField tmp(len);
	for (register int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (register int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (bf.BitLen > BitLen) len = bf.BitLen;
	TBitField tmp(len);
	for (register int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (register int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] &= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (register int i = 0; i < BitLen; i++)
	{	
		if (GetBit(i) == 0)
			tmp.SetBit(i); 
		else tmp.ClrBit(i);
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char res;
	for (register int i = 0; i < bf.BitLen; i++)
	{
		while (res != ' ') //пока не введен пробел
		{
			istr >> res;
			if (res != 0) bf.SetBit(i); //всё, что не ноль - единица
			else bf.ClrBit(i);
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (register int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}