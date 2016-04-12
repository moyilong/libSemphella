#include "define.h"
#include <libSemphella/utils.h>
#include <math.h>
#include <stdlib.h>
int iops;
bool quiet = false;
uint64_t cc_count = 0;
uint64_t xc_count = 0;
bool status = true;
#undef max
uint64_t tout = numeric_limits<uint64_t>::max();

bool subRun(XPOINT point, int steps)
{
	for (int dg_step = 0; dg_step < DGST_STEP; dg_step++)
	{
		float dgst = (360 / DGST_STEP)*dg_step;
		for (int len_step = 0; len_step < STEPS; len_step++)
		{
			XPOINT target = VectorPoint(point, dgst, steps);
			if (steps <= STEPS && inPoint(target))
				subRun(target, steps + 1);
			else
			{
				char bit_data = chess[(int)point.x][(int)point.y];
				char bit_data2 = chess[(int)point.y][(int)point.x];
				bit_data ^ bit_data2;
			}
			cc_count++;
		}
	}
	return false;
}
time_t beg;
void ThreadMonitor()
{
	if (quiet)
		return;
	while (true)
	{
		esleep(1000);
		double precent = (double)xc_count / (double)(AREA_MAX*AREA_MAX*LOOP_ADD);
		iops = (iops + (xc_count / (time(0) - beg))) / 2;

		ShowProcessBar(precent, ull2s(iops / 1000) + " KIPS");
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
	for (int n = 0; n < LOOP_ADD && time(0)-beg<tout; n++)
	{
#pragma omp parallel for
		for (int x = 0; x < AREA_MAX; x++)
			for (int y = 0; y < AREA_MAX; y++)
			{
				xc_count++;
				subRun(XPOINT(x, y), 0);
			}
	}
	status = false;
}
#include <thread>
void Run()
{
	cout << endl << endl;
	beg = time(0);
	//omp_set_num_threads(2);
/*#pragma omp parallel for
	for (int n = 0; n < 2; n++)
	{
		cout << "TID:" << n << endl;
		if (n == 0)
			_Run();
		else
			ThreadMonitor();
	}*/
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
		iops = (iops + (xc_count / (time(0) - beg))) / 2;
}