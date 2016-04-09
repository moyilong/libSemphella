#include "kernel.h"
#include <time.h>
#include "sum.h"
#include "config.h"
bool kernel_inited = false;
bool debug_stat = __DEFAULT_DEBUG_STAT;
kernel KERNEL;

void kernel::SetDebugStat(bool stat,string file,int line)
{
#ifndef __ALLOW_DEBUG_STAT_CHANGE
	message("Debug stat change was been disabled! Track from " + file);
#else
	debug_stat = stat;
#endif
}


bool kernel::GetDebugStat()
{
	return debug_stat;
}

kernel::kernel()
{
	message("Kernel Start!");
	if (kernel_inited)
		this->abort();
	start_time = time(0);
	for (int n = 0; n < init_call.size(); n++)
		init_call.at(n)();
}


void kernel::LogoPrint()
{
	cout << "                 ___         ___                ___  \n               /      /     /   /   /\\    /   /      \n              /----  /     /   /   /  \\  /   /----   \n             /____  /___  /___/   /    \\/   /____    \n";
}

void kernel::Register(REG_TYPE reg, KSAPI api)
{
	switch (reg)
	{
	case INIT:
		init_call.push_back(api);
		break;
	case INEXIT:
		exit_call.push_back(api);
		break;
	}
}


kernel::~kernel()
{
	for (int n = 0; n < exit_call.size(); n++)
		exit_call.at(n)();

}

time_t kernel::get_start_time()
{
	return time(0) - start_time;
}

void kernel::abort()
{
	this->error("System Faild Was benn Called!");
	exit(-1);
}
void kernel::message(string info, string file, int line)
{
	if (debug_stat) cout << "[" << file << "][" << line << "]" << info << endl;
}
void kernel::error(string info, string file, int line)
{
	cout << "[ERROR][" << file << "][" << line << "]" << info << endl;
	exit(-1);
}
