#include "define.h"




bool inPoint(XPOINT point)
{
	if (point.x > 0 && point.x < AREA_MAX && point.y>0 && point.y < AREA_MAX)
		return true;
	return false;
}