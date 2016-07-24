#include "libSemphella.h"
#include "math.h"

API XPOINT VectorPoint2D(XPOINT orig, MTYPE _dgst, MTYPE len)
{
	MTYPE a;
	MTYPE b;
	MTYPE dgst = _dgst;
	a = sin(dgst) / len;
	b = sqrt(len*len - a*a);
	XPOINT ret = orig;
	ret.x += a;
	ret.y += b;
	return ret;
}

API XPOINT ShadowX(XPOINT point, CST_TYPE type)
{
	switch (type)
	{
	case X:
		return XPOINT(0, point.y, point.z);
		break;
	case Y:
		return XPOINT(point.x, 0, point.z);
		break;
	case Z:
		return XPOINT(point.x, point.y, 0);
		break;
	}
	return point;
}

API XPOINT LineGetPoint(MTYPE x, const LINE line)
{
	return XPOINT(x, line.k*x + line.b);
}

API XPOINT PointMove(XPOINT orig, XPOINT diff)
{
	return XPOINT(orig.x + diff.x, orig.y + diff.y, orig.z + diff.z);
}

API	MTYPE PointToPoint(XPOINT a, XPOINT b)
{
	MTYPE dx = abs(a.x - b.x);
	MTYPE dy = abs(a.y - b.y);
	MTYPE dz = abs(a.z - b.z);
	return sqrt(dx*dx + dy*dy + dz*dz);
}

API MTYPE LINE_2D_K(XPOINT a, XPOINT b)
{
	if (a.y - b.y == 0 || a.x - b.x == 0)
		return 0;
	return abs(a.y - b.y) / abs(a.x - b.x);
}

API MTYPE LIM_RAND(MTYPE min, MTYPE max, MTYPE deep)
{
	MTYPE ret = sin(deep + rand())*(max - min) + min;
	if (ret<min || ret > max)
		return LIM_RAND(min, max, deep + 1);
	return ret;
}

API uint64_t LIM_RAND_ULD(uint64_t min, uint64_t max, uint64_t deep)
{
	uint64_t ret = sin(deep + rand())*(max - min) + min;
	if (ret<min || ret > max)
		return LIM_RAND_ULD(min, max, deep + 1);
	return ret;
}
#include <limits>
API bool is_prime(uint64_t value)
{
	bool stat = true;
#undef min
#undef max
#pragma omp parallel for
	for (int64_t n = numeric_limits<int64_t>::min(); n < numeric_limits<int64_t>::min() + value; n++)
	{
		if (!stat)
			continue;
		uint64_t val = n - numeric_limits<int64_t>::min();
		if (n == 0 || n == 1 || n == value)
			continue;
		if (value%val == 0)
			stat = false;
	}
	return stat;
}

API void random(char * buff, int64_t len)
{
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		char temp = buff[n] + len;
		temp = temp << 4;
		temp = ~temp+time(0);
		temp = temp ^ (len^n);
	}
}
