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