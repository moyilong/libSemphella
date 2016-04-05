#pragma once
#include <vector>
#include <cmath>
#include <math.h>

template<class T>inline T dZero(T n, T def = 1)
{
	if (n == 0)
		return def;
	return n;
}

inline bool isZero(void *prt, size_t len)
{
	char *ptr = (char*)prt;
	bool ret = true;
	for (int n = 0; n < len; n++)
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

#ifndef MTYPE
#define MTYPE	float
#endif

struct XPOINT {
	MTYPE x;
	MTYPE y;
	MTYPE z;
	inline XPOINT(MTYPE _x, MTYPE _y)
	{
		x = _x;
		y = _y;
	}
	inline XPOINT(MTYPE _x, MTYPE _y, MTYPE _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

API XPOINT VectorPoint2D(XPOINT orig, MTYPE _dgst, MTYPE len);

#define VectorPoint VectorPoint2D
#define MT_MAX	numeric_limits<MTYPE>::max();
#define MT_MIN	numeric_limits<MTYPE>::min();

API XPOINT PointMove(XPOINT orig, XPOINT diff);