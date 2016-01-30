#include "utils.h"
#include "math.h"

API void ShowProcessBar(double percent, string display, char finish , char splite, char inprocess, int bis )
{
	string finishd;
	string inprocessd;
	int fs = bis*percent;
	int is = bis - fs;
	for (int n = 0; n < fs; n++)
		finishd += finish;
	for (int n = 0; n < is; n++)
		inprocessd += inprocess;
	finishd += splite;
	char buff[MAX_BUFF_SIZE] = { 0x00 };
	sprintf(buff, "%d", (int)(percent * 100));
	string end = buff + (string)"%[" + finishd + inprocessd + "] " + display + "      ";
	cout << end << "\r";
}
