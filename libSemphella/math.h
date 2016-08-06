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

template<class T> inline T eabs(T value)
{
	if (value < 0)
		return value - 2 * value;
	return value;
}

#ifndef MTYPE
#define MTYPE	float
#endif

enum CST_TYPE {
	X,
	Y,
	Z
};

struct API XPOINT {
	MTYPE x;
	MTYPE y;
	MTYPE z;
	XPOINT();
	XPOINT(MTYPE _x, MTYPE _y);
	XPOINT(MTYPE _x, MTYPE _y, MTYPE _z);
	bool equal(const XPOINT b);
	uint64_t hash();
};

struct LIMIT_LINE {
	XPOINT a;
	XPOINT b;
	inline LIMIT_LINE() {};
	inline LIMIT_LINE(XPOINT _a, XPOINT _b)
	{
		a = _a;
		b = _b;
	}
};
API XPOINT VectorPoint2D(XPOINT orig, MTYPE _dgst, MTYPE len);

#define VectorPoint VectorPoint2D
#define MT_MAX	numeric_limits<MTYPE>::max();
#define MT_MIN	numeric_limits<MTYPE>::min();

API XPOINT ShadowX(XPOINT point, CST_TYPE type);

struct LINE {
	MTYPE k = 1;
	MTYPE b = 0;
	inline LINE() {};
	inline LINE(MTYPE xk, MTYPE xb)
	{
		k = xk;
		b = xb;
	}
};

API XPOINT LineGetPoint(MTYPE x, const LINE line);
API	MTYPE PointToPoint(XPOINT a, XPOINT b);
API MTYPE LINE_2D_K(XPOINT a, XPOINT b);
API MTYPE LIM_RAND(MTYPE min, MTYPE max, MTYPE deep = 0);
API uint64_t LIM_RAND_ULD(uint64_t min, uint64_t max, uint64_t deep = 0);
API bool is_prime(uint64_t value);
API void random(char *buff, int64_t len, uint64_t loop_size=512);

inline uint64_t random()
{
	char buff[sizeof(uint64_t)];
	random(buff, sizeof(uint64_t));
	uint64_t ret;
	memcpy(&ret, buff,sizeof(uint64_t));
	return ret;
}

struct API block_math {
	uint64_t begin_offset;
	uint64_t block_size;
	uint64_t block_append;
	uint64_t Addr2Block(uint64_t addr, uint64_t &blk, uint64_t &off,bool with_head=false);
};


API vector<LIMIT_LINE> ShortX(vector<LIMIT_LINE> data);
API void ShortXTest(uint64_t test_numbers,string outformal="%ld,%ld,%ld ,, %ld,%ld,%ld\n");
API void random_test();