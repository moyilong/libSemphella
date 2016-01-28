#include "utils.h"
#include "math.h"
API void ShowProcessBar(double percent, string display, char finish,char splite, char inprocess,int bis)
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
	finishd.at(finishd.size() - 1) = splite;
	char buff[MAX_BUFF_SIZE] = { 0x00 };
	sprintf(buff,"%d", (int)percent * 100);
	string end = buff + (string)"%[" + finishd + inprocessd + "] " + display+ "      ";
	cout << end << "\r";
}

API void ProcessArgment(int argc, char * argv[], vector<ARGMENT>& poll)
{
	for (int n = 0; n < argc; n++)
		if (argv[n][0] == '-')
		{
			ARGMENT temp;
			temp.name = argv[n] + 1;
			if (n + 1 < argc)
			{
				if (argv[n + 1][0] == '-')
					temp.value = "y";
				else
				{
					temp.value = argv[n + 1];
					n++;
				}
			}
			else
				temp.value = "y";
			poll.push_back(temp);
		}
}

API bool HaveArgment(string name, vector<ARGMENT>& poll)
{
	int xn = -1;
#pragma omp parallel for
	for (int n = 0; n < poll.size(); n++)
		if (poll.at(n).name == name)
			xn = n;
	if (xn != -1)
		return true;
	else
		return false;
}

API string GetValue(string name, vector<ARGMENT>& poll)
{
	int xn = -1;
#pragma omp parallel for
	for (int n = 0; n < poll.size(); n++)
		if (poll.at(n).name == name)
			xn = n;
	if (xn != -1)
		return poll.at(xn).value;
	else
		return "n";
}

bool ARGMENT::is_bool()
{
	return value == "y";
}
