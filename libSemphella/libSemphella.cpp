
#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>
using namespace std;
#include "libSemphella.h"
#ifndef __LINUX__
#include <Windows.h>
#endif

API void sZero(void *dest,int value, int len)
{
	char*_dest = (char*)dest;
	for (int n = 0; n < len; n++)
		_dest[n] = value;
}

