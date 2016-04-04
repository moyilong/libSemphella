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
			}
		}
	cout << "Running Memory Write Test...";
	memTest = time(0);
	init();
	memTest = time(0) - memTest;
	cout << "OK" << endl << "Timeout:" << memTest << endl;
	cout << "Running Main Test..."<<endl;
	algorthimTest = time(0);
	Run();
	algorthimTest = time(0) - algorthimTest;
	cout << "End!" << endl;
	cout << "Main Algorthim Performance:" << iops << endl;
}