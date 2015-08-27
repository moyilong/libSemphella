#include "libDragonEDGE.h"
#include "utils.h"
bool diff_check(IO_BUFF data)
{
	return data.sum == xbit((char*)&data, IO_LEN - sizeof(data.sum));
}

void write_check(IO_BUFF &data)
{
	data.sum=xbit((char *)&data, IO_LEN-sizeof(data.sum));
}

