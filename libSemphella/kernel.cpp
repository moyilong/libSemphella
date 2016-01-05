#include "kernel.h"
#include <time.h>

bool kernel_inited = false;

kernel KERNEL;

kernel::kernel()
{
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
	cout << "libSemphella Kernel was been crashed!" << endl;
	exit(-1);
}