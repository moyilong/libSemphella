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
	cout << "���߳�����:" <<mt_per << " ǧ��ÿ��" << endl;
	cout << "���̲߳���:" << mt_all << " ǧ��" << endl;
	cout << "��������: " << al_per << " ʮ��ÿ��" << endl;
	cout << "���岽��:" << al_all << " ʮ��" << endl;
	cout << "=======================================================" << endl;
	cout << "���ճɼ�:" << final_per << " ǧ��ÿ��" << endl;
	cout << "�Ա�MT7620(580MHZ 128MB DDR2 7.5ǧ��ÿ��):" << div <<" ��"<< endl;
}