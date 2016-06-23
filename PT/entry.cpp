#include "stdafx.h"
#include "define.h"
time_t memTest;
time_t algorthimTest;
extern int steps;
bool signal = false;
bool new_algr = false;
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
			case '2':
				new_algr = true;
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
	cout << "Performance Test Utils" << endl;
	cout << "Running Memory Write Test...";
	memTest = time(0);
	init();
	memTest = time(0) - memTest;
	cout << "OK" << endl << "Timeout:" << memTest << endl;
	cout << "Running Main Test..." << endl;
	algorthimTest = time(0);
	if (!new_algr)
		Run();
	else
		Run2();
	algorthimTest = time(0) - algorthimTest;
	if (algorthimTest == 0)
		algorthimTest = 1;
	cout << endl << endl;
	double final_per = iops / algorthimTest / (double)1000;
	final_per *= (omp_get_num_threads() / 10) + 1;
	cout << "Final Score:" << final_per << " KS/S" << endl;
	if (!new_algr)
	{
		double div = final_per / MT7620_PERFORMANCE;
		cout << "Performance to MT7620(580MHZ 128MB DDR2 7.5KS/S):" << div << " Divid" << endl;
	}
}