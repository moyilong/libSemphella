#pragma once
#include "main.h"
API void sZero(void *dest, int value, int len);

#define DEBUG_LINE if (false)

#ifdef __LINUX__
#define esleep(ms) usleep(ms*1000)
#else
#define esleep(ms) Sleep(ms)
#endif