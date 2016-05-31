#include "time.h"
#include <time.h>
#include <sys/timeb.h>
API uint64_t gettimems()
{
	struct timeb t1;
	ftime(&t1);
	return t1.millitm;
}