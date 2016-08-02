#include "utils.h"
#include "math.h"
#include "string.h"

API  void ShowProcessBar(double _percent, string display, char finish, char splite, char inprocess, int bis)
{
	double percent = _percent;
	if (percent >= 100)
		percent = percent/100;
	int proceed = bis*percent;
	//int incomplete = bis - proceed - 1;
	//cout << percent * 100 << "%[";
	/*printf("%.2lf %[", percent * 100);
	for (int n = 0; n < proceed; n++)
		cout << finish;
	cout << splite;
	for (int n = 0; n < incomplete; n++)
		cout << inprocess;
	cout << "]  " << display << "       " << "\r";*/
	char *buff = (char*)malloc(bis);
	memset(buff, finish, proceed);
	if (bis - proceed > 0)
	{
		memset(buff + proceed+1, inprocess, bis - proceed);
		if (proceed +1 < 100)
			buff[proceed +1] = splite;
	}
	cout << display << " [" << buff << "]                   \r";
}

API void ShowProcessBarEx(int all, int st_1, int st_2, string display, char st1_ch, char st2_ch, char st3_ch)
{
	string finished = "[";
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

API void ValueDisplay(double data, string tips, ValueInfo &info, char end_char)
{
#undef min
#undef max
	string out = "%s [VAL %.2lf %.2lf(%%%.2lf)]\t[AVG %.2lf %.2lf (%%%.2lf)]\t[MAX: %.2lf]\t[MIN: %.2lf]";
	out += end_char;
	if (info.all_count == 0)
	{
		info.avg = data;
		info.all_count++;
		info.min = data;
		info.max = data;
		info.last = data;
		return;
	}
	double avg = dZero(info.avg + data) / 2;
	double avg_diff = avg - info.avg;
	double dir_diff = data - info.last;
	double ad_per = dZero(avg_diff) / dZero(avg);
	double dd_per = dZero(dir_diff) / dZero(data);
	info.avg++;
	//info.avg = (info.avg + data) / 2;

	info.min = emin(data, info.min);
	info.max = emax(data, info.max);
	printf(out.data(),tips.data(), data, dir_diff, dd_per * 100, avg, avg_diff, ad_per * 100, info.min, info.max);
	info.last = data;
	info.avg = avg;
	info.all_count++;
}

void ValueInfo::clean()
{
	avg = 0;
	max = 0;
	min = 0;
	all_count = 0;
	last = 0;
}

ValueInfo::ValueInfo()
{
	clean();
}
