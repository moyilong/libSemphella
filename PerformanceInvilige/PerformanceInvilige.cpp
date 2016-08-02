// PerformanceInvilige.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <TAHITI/stdafx.h>

#define TEST_LEN	8192
#define LOOP_LEN	16384*16
#define CDATA_LEN	SEND_LEN
#define PASSWORD_AREA	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/*-+~`!@#$%%^&*()_+-="
#include <time.h>
string bool2str(bool a)
{
	if (a)
		return "true";
	else
		return "false";
}
int main(int argc, char *argv[])
{
	cout << "===========================================" << endl;
	cout << "TAHITI Protoco Version " << S_API_VERSION << endl;
	cout << "TAHITI Procoto API Level:" << S_API_ALLOW_MIN << endl;
	cout << "Protoco Static Send Len:" << SEND_LEN << endl;
	cout << "Protoco Static Data Len:" << DATA_LEN << endl;
	cout << "Crypt Code Len:" << AUTH_CODE_LEN << endl;
	cout << "===========================================" << endl;
	if (ENDIAN_TEST() == LIT_ENDIAN)
		cout << "Little Endian Protoco" << endl;
	else
		cout << "Big Endian Protoco" << endl;
	cout << "===========================================" << endl;
	if (kernel().old_protoco_version)
		cout << "Use Old Protoco" << endl;
	else
		cout << "Use New Dymanic Protoco" << endl;
	cout << "===========================================" << endl;
	cout << "Kernel Information:" << endl;
	cout << "Kernel Mode:" << kernel().kernel_mode << endl;
	cout << "kernel.max_try_conn=" << kernel().max_try_conn << endl;
	cout << "kernel.try_wait=" << kernel().try_wait << endl;
	cout << "kernel.quiet=" << bool2str(kernel().quiet) << endl;
	cout << "kernel.old_protoco_version=" << bool2str(kernel().old_protoco_version) << endl;
	cout << "kernel.allow_server_back_level=" << bool2str(kernel().allow_server_back_level) << endl;
	cout << "===========================================" << endl;
	cout << "Start Performance Test:" << endl;
	cout << "ProcessKeyLen:" << TEST_LEN << endl;
	cout << "Data Len:" << CDATA_LEN << endl;
	cout << "size of unsigned long long:" << sizeof(unsigned long long) << endl;
	cout << "size of uint64:" << sizeof(uint64_t) << endl;
	string password;
	char process_buff[CDATA_LEN];
	for (int n = 0; n < TEST_LEN; n++)
	{
		password += PASSWORD_AREA[(int)(double)sin(n)*strlen(PASSWORD_AREA)];
	}
	for (int n = 0; n < CDATA_LEN; n++)
	{
		process_buff[n] = rand();
	}
	clock_t start = clock();
#pragma omp parallel for
	for (int n = 0; n < LOOP_LEN; n++)
		xorcrypt(process_buff, CDATA_LEN, password);
	clock_t stop = clock();
	double score = 1000 * (stop - start) / LOOP_LEN;
	cout << "Clock Event Time Out:" << score << endl;
#ifndef __LINUX__
	system("pause");
#endif
	return 0;
}