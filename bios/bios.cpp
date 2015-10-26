// bios.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
#include <libSemphella/libSemphella.h>
#include <libSemphella/libbios.h>
string emufile = "bios.bin";
#define default_sign "ELONE DragonOS moyilong Phenom Semphella libDragon"

int main(int argc, char *argv[])
{
	bool create = false;
	string sign = default_sign;
	
	for (int n = 1; n < argc - 1; n++)
		if (argv[n][0] == '-')
		{
		switch (argv[n][1]){
		case 's':
			n++;
			sign = argv[n];
			break;
		case 'c':
			create = true;
			break;
		case 'o':
			cout << OS_TYPE << endl;
			exit(0);
			break;
		}
		}
	emufile = argv[argc - 1];
	BIOS data(emufile, sign,create);
	if (data.last_stat != NORMALLY)
	{
		cout << "IO Error!" << endl;
		exit(-1);
	}
	for (int n = 1; n < argc - 1; n++)
		if (argv[n][0] == '-')
		{
		switch (argv[n][1])
		{
		case 'W':
			data.write();
			break;
		case 'r':
			n++;
			printf("%ul", data.readenv(atoi(argv[n])));
			exit(0);
			break;
		case 'w':
			data.setenv(atoi(argv[n + 1]), atoll(argv[n + 2]));
			n += 2;
			break;
		case 'd':
			for (int x = 0; x < DSDT_LEN; x++)
				printf("%d\t\t%ul", x, data.readenv(x));
			exit(0);
			break;
		case 'e':
			for (int b = 0; b < DSDT_LEN; b++)
				data.setenv(b, 0x00);
			exit(0);
			break;
		case 'v':
			cout << hex << data.info.vendor << ":" << data.info.product << oct << endl;
			exit(0);
			break;
		case 'V':
			cout << "BIOS Version String:" << endl << data.info.version_info << endl;
			exit(0);
			break;
		case 'O':
			cout << data.info.OS << ":" << OS_TYPE << endl;
			exit(0);
			break;
		case 'u':
			if (data.info.EFI.first_init_time == -1)
				data.info.EFI.first_init_time = time(0);
			break;
		case 'D':
			switch (argv[n][2])
			{
			case 'S':
				data.info.EFI.power_up_count++;
				data.write();
				break;
			case 'H':
				data.info.EFI.power_up_time++;
				data.write();
				break;
			case 's':
				cout << "Start Count:" << data.info.EFI.power_up_count << endl;
				break;
			case 'h':
				cout << "Start Time:" << data.info.EFI.power_up_time << endl;
				break;
			default:
				cout << "Unknow Command Mode!" << endl;
				break;
			}
			break;
		case 'I':
			cout << data.info.version_info << endl;
			cout << "PID:"<<hex<<data.info.product <<oct<< endl;
			cout << "VID:" << hex<<data.info.vendor<<oct << endl;
			//cout << "First Init Time:" << data.info.EFI.first_init_time << endl;
			printf("First Init Time:%lld\n", data.info.EFI.first_init_time);
			cout << "Start Count:" << data.info.EFI.power_up_count << endl;
			break;
		case 'c':
		case 's':
		case 'o':
			break;
		default:
			cout << "Unknow Command!" << endl;
			break;
		}
		}
}
