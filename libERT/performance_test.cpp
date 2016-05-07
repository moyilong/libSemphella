#include "algorthim.h"
#include "fhandle.h"
#include "stdafx.h"
struct SCORE_T {
	long double time_out;
	int id;
	uint64_t all_count;
	uint64_t timeout;
};
int timeout = 2;
#define TEST_LOOPS	128
SCORE_T runOnce(int id)
{
	debug << "Prepering for Test:" << APOLL[id].id << endl;
	char buff[MAX_BUFF_SIZE];
	uint64_t hash = getsumV2(buff, MAX_BUFF_SIZE);
	char pwd[MAX_BUFF_SIZE];
	sprintf(buff, "%ull", hash);
	for (int n = 0; n < MAX_BUFF_SIZE; n++)
		buff[n] = rand();
	uint64_t t_hash_1, t_hash_2;
	uint64_t count = 0;
	time_t start = time(0);
	while (time(0) - start <= timeout)
	{
		count++;
		APOLL[id].pa(pwd);
		t_hash_1 = APOLL[id].sa(buff, MAX_BUFF_SIZE);
		APOLL[id].ca(buff, MAX_BUFF_SIZE, 0);
		APOLL[id].ca(buff, MAX_BUFF_SIZE, 0);
		t_hash_2 = APOLL[id].sa(buff, MAX_BUFF_SIZE);
	}
	time_t stop = time(0);
	SCORE_T sc;
	sc.id = APOLL[id].id;
	sc.time_out = count / (stop - start);
	sc.timeout = stop - start;
	sc.all_count = count;
	return sc;
}
LIBERT_API void LIB_ERTLIB::PerformanceTest()
{
	cout << "Start Performance Test!" << endl;
	cout << "Algorthim Size:" << APOLL_SIZE << endl;
	vector<SCORE_T>poll;
	for (int n = 0; n < APOLL_SIZE; n++)
	{
		int id = APOLL[n].id;
		debug << "Initing Algorthim:" << id << endl;
		if (APOLL[n].can_be_pt)
		{
			SCORE_T sc = runOnce(n);
			poll.push_back(sc);
			cout << "ID:" << id << "\tTimtout:" << sc.time_out << " IOPS\tof " << sc.all_count << " IOS \tin " << sc.timeout << " SECOND" << endl;
		}
		else {
			cout << "ID:" << id << "\tUnsupported!" << endl;
		}
	}
	debug << "Sorting..." << endl;
	SCORE_T *sct = (SCORE_T*)malloc(sizeof(SCORE_T)*poll.size());
	for (int n = 0; n < poll.size(); n++)
		sct[n] = poll.at(n);
	bool stat = true;
	while (stat)
	{
		stat = false;
		for (int n = 0; n < poll.size() - 1; n++)
		{
			if (sct[n].time_out < sct[n + 1].time_out)
			{
				SCORE_T temp = sct[n + 1];
				sct[n + 1] = sct[n];
				sct[n] = temp;
				stat = true;
			}
		}
	}
	cout << "=======================Result====================" << endl;
	for (int n = 0; n < poll.size(); n++)
	{
		if (n == 0)
			cout << "*";
		else
			cout << " => ";
		cout << sct[n].id;
	}
}
	