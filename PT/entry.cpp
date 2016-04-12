#include "stdafx.h"
#include "define.h"
time_t memTest;
time_t algorthimTest;

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
				omp_set_num_threads(1);
				break;
			case 't':
				n += 1;
				tout = atoi(argv[n]);
				break;
			}
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
	cout << "Main Algorthim Performance:" << xc_count / algorthimTest / 1000 << " KIPS" << endl;
	cout << "All Mainline Caclulate Size:" << xc_count / 1000 << " KIO" << endl;
	cout << "Caculate Thread Performance : " << cc_count / algorthimTest / 1000000 << " MIPS" << endl;
	cout << "All Subline Caculate Size:" << cc_count / 1000000 << " MIO" << endl;
	cout << "=======================================================" << endl;
	cout << "Final Score:" << cc_count / algorthimTest / 1000 << " KIPS" << endl;
	cout << "for MT7620N (580MHZ 128MB DDR2 1500IOPS):" << cc_count / algorthimTest / MT7620_PERFORMANCE << endl;
}