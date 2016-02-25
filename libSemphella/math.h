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