#include "define.h"

char chess[AREA_MAX][AREA_MAX];

void init()
{
#pragma omp parallel for
	for_each
	{
		if (rand() % 2 == 0)
		chess[x][y] = 0;
		else
			chess[x][y] = 1;
	}
}