#include "define.h"
#include <libSemphella/utils.h>
int iops;
bool quiet = false;
bool Run(XPOINT point, int steps)
{
#pragma omp parallel for
	for (int dg_step = 0; dg_step < DGST_STEP; dg_step++)
	{
		float dgst = (360 / DGST_STEP)*dg_step;
#pragma omp parallel for
		for (int len_step = 0; len_step < STEPS; len_step++)
		{
			XPOINT target = VectorPoint(point, dgst, steps);
			if (steps <= STEPS && inPoint(target))
				Run(target, min(0,steps) + 1);
		}
	}
}


void Run()
{
	char buff[MAX_BUFF_SIZE];
	cout << endl << endl;
	uint64_t count = 0;
	time_t beg = time(0);
	for_each
	{
		count++;
		Run(XPOINT(x,y),0);
		double precent = (double)count / (double)(AREA_MAX*AREA_MAX);
		if (count % 20 == 0&&!quiet)
	if ((int)precent % AREA_MAX == 0)
	{
		if (time(0) != beg)
			iops = (iops + (count / (time(0) - beg))) / 2;
		memset(buff, 0, sizeof(buff));
		sprintf(buff, " %dIOPS",iops);
		ShowProcessBar(precent, buff);

	}
	}
		if (time(0) == beg)
			iops = -1;
		else
			iops = (iops + (count / (time(0) - beg))) / 2;
}