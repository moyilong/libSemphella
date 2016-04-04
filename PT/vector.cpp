#include "define.h"


XPOINT VectorPoint(XPOINT orig, float _dgst, float len)
{
	float a;
	float b;
	float dgst = _dgst;
	a = sin(dgst) / len;
	b = sqrt(len*len - a*a);
	XPOINT ret = orig;
	ret.x += a;
	ret.y += b;
	return ret;
}

bool inPoint(XPOINT point)
{
	if (point.x > 0 && point.x < AREA_MAX && point.y>0 && point.y < AREA_MAX)
		return true;
	return false;
}