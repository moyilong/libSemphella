#include "utils.h"
#include "math.h"
#include "string.h"


API  void ShowProcessBar( double percent, string display, char finish , char splite , char inprocess , int bis )
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
	//char buff[16];
	//sprintf(buff, "%d", (int)percent * 100);
	string end = int2s(abs((int)(percent*100))) + (string)"%[" + finishd + inprocessd + "] " + display + "      ";
	cout << end << "\r";
}

API void ShowProcessBarEx(int all, int st_1, int st_2,string display, char st1_ch, char st2_ch, char st3_ch)
{
	string finished="[";
	for (int n = 0; n < all; n++)
	{
		if (n <= st_1)
			finished += st1_ch;
		else if (n <= st_2)
			finished += st2_ch;
		else
			finished += st3_ch;
	}
	finished += "]" + display;
	cout << finished << endl;

}