// PT2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/argment.h>
#include <thread>
argment args;
sysconfig cfg;
vector<int> poll;
void for_each_args(string name, string val)
{
	switch (name[0])
	{
	case 't':
		cfg.time = atoi(val.data());
		break;
	case 'o':
		cfg.based_offset = atoi(val.data());
		break;
	case 'u':
		cfg.un_excepted = true;
		break;
	case 's':
		omp_set_num_threads(1);
		break;

	}
}
uint64_t iops = 0;
void main_args()
{
	cfg.begin = time(0);
	while (true)
	{
		cfg.based_offset++;
		if (!cfg.un_excepted)
			if (time(0) - cfg.begin > cfg.time)
			{
				break;
			}
		if (cfg.based_offset == 0 || cfg.based_offset == 1 || cfg.based_offset == 2)
		{
			poll.push_back(cfg.based_offset);
			continue;
		}
		bool checked = false;
#pragma omp parallel for
		for (int n = 2; n < cfg.based_offset; n++)
		{
			iops++;
			if (cfg.based_offset%n == 0)
			{
				if (cfg.based_offset != 0 || cfg.based_offset != 1 || cfg.based_offset != n)
					checked = true;
			}
		}
		if (checked)
			poll.push_back(cfg.based_offset);
	}
}
thread calcu_thread(main_args);

void monitor_thread()
{
	ValueInfo info;
	uint64_t value=0;
	while (calcu_thread.joinable())
	{
		if (value == 0)
		{
			value = iops;
			continue;
		}
		
		ValueDisplay(iops-value, "",info,'\n');
		value = iops;
		esleep(500);
	}
	cfg.stop = time(0);
}
thread monitor_threads(monitor_thread);
int main(int argc, char *argv[])
{
	args.load(argc, argv);
	args.for_each(for_each_args);
	int beg = cfg.based_offset;
	calcu_thread.join();
	monitor_threads.join();
	cout << "End Count!" << endl;
	cout << "FindVal:" << poll.size() << endl;
	cout << beg << " => " << cfg.based_offset << endl;
	cout << "Timeout:" << cfg.stop - cfg.begin << endl;
	cout << "Performance:" << iops / (cfg.stop - cfg.begin) << endl;
}