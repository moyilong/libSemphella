#include "utils.h"
#include "math.h"
#include "string.h"

API  void ShowProcessBar(float _percent, string display, char finish, char splite, char inprocess, int bis)
{
	double val = _percent;
	if (val >= 1)
		val = val / 100;
	int comp = val*bis;
	char *buff = (char*)malloc(bis+1);
	memset(buff, inprocess, bis);
	memset(buff, finish, comp);
	buff[bis] = '\0';
	buff[comp] = splite;
	cout << StrLimit(display,8,true) << " [" << buff << "] \r";
	free(buff);
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
	string out = "%s [VAL %.2lf %.2lf(%.2lf%%)][AVG %.2lf %.2lf (%.2lf%%)][MAX: %.2lf][MIN: %.2lf]";
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
#include <string.h>
#include <sys/types.h>
#ifndef __LINUX__
#define popen _popen
#endif

API string shell(string cmds)
{
	string ret;
	FILE *fp = popen(cmds.data(), "r");
	if (fp == NULL)
		return "UN_EXECED";
	char buff[128];
	while (fgets(buff, 128, fp))
		ret += buff;
	return ret;
}

API void HexDump(const char *ptr, uint64_t len)
{
	char buff[17];
	for (uint64_t n = 0; n < len; n++)
	{
		if (n % 16 == 0)
		{
			if (n == 0)
				cout << endl;
			else
				cout << buff << endl;
		}
		printf("0x%02x ", (unsigned int)ptr[n]);
		if (ptr[n] >= 0x21 && ptr[n] <= 0x7E)
			buff[n % 16] = ptr[n];
	}
}