#pragma once
#include "main.h"
#include "libSemphella.h"

#include "math.h"

inline void ShowProcessBar(double percent, string display, char finish='=', char splite='>', char inprocess=' ', int bis=32)
{
	string finishd;
	string inprocessd;
	int fs = bis*percent;
	int is = (1 - bis)*percent;
	for (int n = 0; n < (int)max(fs, is); n++)
	{
		if (n <= fs)
			finishd += finish;
		if (n <= is)
			inprocessd += inprocess;
	}
	finishd += splite;
	char buff[MAX_BUFF_SIZE] = { 0x00 };
	sprintf(buff, "%d", (int)(percent * 100));
	string end = buff + (string)"%[" + finishd + inprocessd + "] " + display + "      ";
	cout << end << "\r";
}
