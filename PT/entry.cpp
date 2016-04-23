#include "stdafx.h"
#include "define.h"
time_t memTest;
time_t algorthimTest;
extern int steps;
bool signal = false;
int main(int argc, char *argv[])
{
	for (int n = 0; n < argc; n++)
		if (argv[n][0] == '-' || argv[n][0] == '/')
		{
			switch (argv[n][1])
			{
			case 'q':
				quiet = true;
				break;
			case 's':
				signal = true;
				omp_set_num_threads(1);
				break;
			case 't':
				n += 1;
				tout = atoi(argv[n]);
				cout << "Setting Timeout:" << tout << endl;
				break;
			case'n':
				steps = atoi(argv[n + 1]);
				n += 1;
				break;
			default:
				cout << "Unknow Options:" << argv[n];
				exit(-1);
			}
		}
	if (!signal)
	{
		omp_set_num_threads(omp_get_max_threads() - 1);
	}
	cout << "Running Memory Write Test...";
	memTest = time(0);
	init();
	memTest = time(0) - memTest;
	cout << "OK" << endl << "Timeout:" << memTest << endl;
	cout << "Running Main Test..." << endl;
	algorthimTest = time(0);
	Run();
	algorthimTest = time(0) - algorthimTest;
	if (algorthimTest == 0)
		algorthimTest = 1;
	cout << endl << endl;
	double mt_per = xc_count / algorthimTest / (double)1000;
	double mt_all = xc_count / (double)1000;
	double al_per = cc_count / algorthimTest / (double)1000000;
	double al_all = cc_count / (double)1000000;
	double final_per = cc_count / algorthimTest / (double)1000;
	double div = cc_count / algorthimTest / MT7620_PERFORMANCE;
	cout << "主线程性能:" <<mt_per << " 千步每秒" << endl;
	cout << "主线程步数:" << mt_all << " 千步" << endl;
	cout << "整体性能: " << al_per << " 十万步每秒" << endl;
	cout << "整体步数:" << al_all << " 十万步" << endl;
	cout << "=======================================================" << endl;
	cout << "最终成绩:" << final_per << " 千步每秒" << endl;
	cout << "对比MT7620(580MHZ 128MB DDR2 7.5千步每秒):" << div <<" 倍"<< endl;
}