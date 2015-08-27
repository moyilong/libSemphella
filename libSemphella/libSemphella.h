#pragma once
#include "main.h"
API bool get_debug_stat();
API void set_debug_stat(bool stat);
API void sZero(void *dest, int value, int len);

#define DEBUG_LINE if (get_debug_stat())
