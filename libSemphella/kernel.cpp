#include "kernel.h"
#include <time.h>
#include "sum.h"
bool kernel_inited = false;
bool debug_stat = true;
kernel KERNEL;
bool kernel::GetDebugStat()
{
	return debug_stat;
}

kernel::kernel()
{
	this->message("Kernel Start!");
	if (kernel_inited)
		this->abort();
	start_time = time(0);
}



kernel::~kernel()
{


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
	cout << "[" << file << "][" << line << "]" << info << endl;
}
void kernel::error(string info, string file, int line)
{
	cout << "[ERROR][" << file << "][" << line << "]" << info << endl;
	exit(-1);
}

struct ALLOC {
	void *alloc;
	char file[128];
	int line;
};