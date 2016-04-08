#include "algorthim.h"
#include "stdafx.h"
struct SCORE_T {
	long double time_out;
	int id;
};
#define TEST_LOOPS	128
SCORE_T runOnce(int id)
{
	char buff[MAX_BUFF_SIZE];
	uint64_t hash = getsumV2(buff, MAX_BUFF_SIZE);
	char pwd[MAX_BUFF_SIZE];
	sprintf(buff, "%ull", hash);
	for (int n = 0;n < MAX_BUFF_SIZE;n++)
		buff[n] = rand();
	uint64_t t_hash_1, t_hash_2;
	uint64_t count = 0;
	time_t start = time(0);
	while (time(0) - start <= 4)
	{
		count++;
		APOLL[id].pa(pwd);
		t_hash_1 = APOLL[id].sa(buff, MAX_BUFF_SIZE);
		APOLL[id].ca(buff, MAX_BUFF_SIZE, 0);
		decrypt = true;
		APOLL[id].ca(buff, MAX_BUFF_SIZE, 0);
		t_hash_2 = APOLL[id].sa(buff, MAX_BUFF_SIZE);
		decrypt = false;
	}
	time_t stop = time(0);
	SCORE_T sc;
	sc.id = id;
	sc.time_out = count / (stop - start);
	decrypt = false;
	return sc;
}

void PerformanceTest()
{
	cout << "Start Performance Test!" << endl;
	cout << "Algorthim Size:" << APOLL_SIZE << endl;
	for (int n = 0;n < APOLL_SIZE;n++)
	{
		int id = APOLL[n].id;
		if (APOLL[n].can_be_pt)
		{
			SCORE_T sc = runOnce(n);
			cout << "ID:" << id << "\tTimtout:" << sc.time_out << " IOPS" << endl;
		}
		else {
			cout << "ID:" << id << "\tUnsupported!" << endl;
		}
	}
}