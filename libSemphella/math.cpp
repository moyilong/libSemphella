
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

API XPOINT PointMove(XPOINT orig, XPOINT diff)
{
	return XPOINT(orig.x + diff.x, orig.y + diff.y,orig.z+diff.z);
}
