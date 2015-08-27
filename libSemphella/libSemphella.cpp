
#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>
using namespace std;
#include "libSemphella.h"

bool debug_stat_locked=false;

#ifdef STATIC_DEBUG

API bool get_debug_stat()()
{
    return DEFAULT_DEBUG_STAT;
}

API void set_debug_stat(){};
#else

bool debug_stat=DEFAULT_DEBUG_STAT;

API bool get_debug_stat()
{
    return debug_stat;
}

API void set_debug_stat(bool stat)
{
    if (!debug_stat_locked)
    debug_stat=stat;
}
#endif

API void into_debug_mode()
{
#ifndef _WNO_OMP
    omp_set_num_threads(1);
#endif
debug_stat_locked=true;
debug_stat=true;
}

API void sZero(void *dest,int value, int len)
{
	char*_dest = (char*)dest;
	for (int n = 0; n < len; n++)
		_dest[n] = value;
}
