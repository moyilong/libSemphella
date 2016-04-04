#pragma once
#include "stdafx.h"
#include "storage.h"
#define AREA_MAX	1024
#define STEPS		16
#define STEP_DIVID	0.5
#define DGST_STEP	360/45
extern char chess[AREA_MAX][AREA_MAX];

#define for_each	 for (int x=0;x<AREA_MAX;x++) for (int y=0;y<AREA_MAX;y++)

extern int iops;
struct XPOINT {
	float x;
	float y;
	inline XPOINT(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
};
XPOINT VectorPoint(XPOINT orig, float _dgst, float len);
bool inPoint(XPOINT point);
bool Run(XPOINT point, int steps);
void init();
void Run();