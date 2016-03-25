#pragma once
#include <vector>
#include <cmath>
#include <math.h>

template<class T>inline T dZero(T n,T def=1)
{
	if (n == 0)
		return def;
	return n;
}

inline bool isZero(void *prt, size_t len)
{
	char *ptr = (char*)prt;
	bool ret = true;
#pragma omp parallel for
	for (int n = 0;n < len;n++)
		if ((char)ptr[n] != 0)
			ret = false;
	return ret;
}

template<class T>inline T emax(const T a, const T b)
{
	if (a > b)
		return a;
	else
		return b;
}

template<class T>inline T emin(const T a, const T b)
{
	if (a < b)
		return a;
	else
		return b;
}