#pragma once
#include "main.h"
API void sZero(void *dest, int value, int len);

#ifndef __LINUX
#include <Windows.h>
#endif

#ifdef __LINUX__
#define esleep(ms) usleep(ms*1000)
#else
#define esleep(ms) Sleep(ms)
#endif

#include "kernel.h"