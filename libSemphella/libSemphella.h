#pragma once
#include "main.h"
API void sZero(void *dest,int value,int len);

inline  void esleep(unsigned long long ms)
{
#ifdef __LINUX__
	usleep(ms*1000);
#else
	Sleep(ms);
#endif
}


#define DEBUG_LINE if (true)
