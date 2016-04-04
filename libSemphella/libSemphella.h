#pragma once
#include "main.h"
API void sZero(void *dest, int value, int len);

#ifdef __LINUX__
#define esleep(ms) usleep(ms*1000)
#else
#include <Windows.h>
#define esleep(ms) Sleep(ms)
#endif

#include "kernel.h"