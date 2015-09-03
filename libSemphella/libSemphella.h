#pragma once
#include "main.h"
#include "kernel.h"
API void sZero(void *dest, int value, int len);

#define DEBUG_LINE if (kernel.get_debug_stat())
