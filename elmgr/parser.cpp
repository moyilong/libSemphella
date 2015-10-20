#include "stdafx.h"
#include "parser.h"
#include "command.h"

void parser(string info)
{
	int count_of_empty = 0;
	string main_exec;
	vector<string> arg;
	for (int n = 0; n < info.size(); n++)
		if (info.at(n) == ' ')
			count_of_empty++;
	if (count_of_empty == 0)
	{
		main_exec = info;
	}
	else{
		vector<int> ipoll;
		for (int n = 0; n < info.size(); n++)
			if (info.at(n) == ' ')
				ipoll.push_back(n);
		for (int n = 1; n < ipoll.size(); n++)
		{
			arg.push_back(info.substr(ipoll.at(n - 1), ipoll.at(n)));
		}
	}
	for (int n = 0; n < get_count(); n++)
	{
		if (streval(get_point(n).get_name().data(), main_exec.data()))
		{
			get_point(n).get_func()(arg);
		}
	}
}

KERNEL kern;

KERNEL &kernel()
{
	return kern;
}

KERNEL::KERNEL()
{
#ifdef __LINUX__
	work_file="/etc/config"
#else
	work_dir = "%cd%";
#endif
}