#include "utils.h"
#include "math.h"
#include "string.h"


API  void ShowProcessBar( double percent, string display, char finish , char splite , char inprocess , int bis )
{
	string str;
	int proceed = bis*percent;
	int incomplete = bis - proceed;
	for (int n = 0; n < proceed; n++)
		str += finish;
	str += splite;
	for (int n = 0; n < incomplete; n++)
		str += inprocess;
	cout << abs((int)(percent*100)) <<"%[" << str<<"]"<<display<<"       " << "\r";
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