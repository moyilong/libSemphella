#include "define.h"
#include <libSemphella/utils.h>
#include <math.h>
#include <stdlib.h>
int iops;
bool quiet = false;
bool status = true;
#undef max
uint64_t tout = -1;
int steps = -1;

bool subRun(XPOINT point, int steps)
{
#pragma omp parallel for
	for (int len_step = 0; len_step < STEPS; len_step++)
	{
		for (int dg_step = 0; dg_step < DGST_STEP; dg_step++)
		{
			MTYPE dgst = (360 / DGST_STEP)*dg_step;
			XPOINT target = VectorPoint(point, dgst, steps);
			if (steps <= STEPS && inPoint(target))
				subRun(target, steps + 1);
			else
			{
				char bit_data = chess[(int)point.x][(int)point.y];
				char bit_data2 = chess[(int)point.y][(int)point.x];
				bit_data ^ bit_data2;
			}
		}
	}
	return false;
}
time_t beg;
time_t time_out;
void ThreadMonitor()
{
	double precent;
	double all = AREA_MAX*AREA_MAX*LOOP_ADD;
	if (quiet)
		return;
	while (true)
	{
		esleep(100);
		time_out = time(0) - beg;
		precent = (double)iops / all;

		ShowProcessBar(precent,  ull2s(time_out) + " s");
		printf("\r");
		if (!status)
		{
			ShowProcessBar(1, "Finish                 \n");
			return;
		}
	}
}

void _Run()
{
	beg = time(0);
	time_out = 0;
	for (int n = 0; n < LOOP_ADD && (time_out < tout || tout == -1 && tout != 0) && (steps == -1 || steps > n); n++)
	{
#pragma omp parallel for
		for (int x = 0; x < AREA_MAX; x++)
			for (int y = 0; y < AREA_MAX; y++)
			{
				subRun(XPOINT(x, y), 0);
				iops++;
			}
	}
	status = false;
}
#include <thread>
void Run()
{
	cout << endl << endl;
	beg = time(0);
	thread ca(_Run);
	thread mo(ThreadMonitor);
	ca.join();
	if (!quiet)
		mo.join();
	if (time(0) == beg)
	{
		cout << "System Append an Error: MAXED_OUT" << endl;
	}
	else
		iops = (iops + ((AREA_MAX*AREA_MAX) / (time(0) - beg))) / 2;
}