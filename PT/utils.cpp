#include "define.h"

//char chess[AREA_MAX][AREA_MAX];

char **chess;

void init()
{
	chess = (char**)malloc(sizeof(char)*AREA_MAX);
	for (int x = 0; x < AREA_MAX; x++)
	{
		chess[x] = (char*)malloc(sizeof(char)*AREA_MAX);
		for (int y = 0; y < AREA_MAX; y++)
		{
			if (rand() % 2 == 0)
				chess[x][y] = 0;
			else
				chess[x][y] = 1;
		}
	}
}