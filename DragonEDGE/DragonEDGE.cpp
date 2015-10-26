// DragonEDGE.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#ifdef __LINUX__
#define PORT	"/dev/ttyUSB0"
#else
#define PORT	"COM3"
#endif

const char send_allow[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";

string port = PORT;
string command = "";

#include "api.h"
const DEVICE_STAT lpstat;
int main(int argc, char *argv[])
{
	for (int n = 1; n < argc; n++)
		if (argv[n][0] == '-')
			switch (argv[n][1])
		{
			case 'p':
				n++;
				port = argv[n];
				break;
			case 'c':
				n++;
				command = argv[n] ;
				break;
			default:
				cout << "Unknow Command:" << argv[n] << endl;
				break;
		}
	IO_TYPE io=pinit(port,lpstat);
	if (io == INVALID_HANDLE_VALUE || io == NULL)
	{
		cout << "Get IO Operate Fiald!" << endl;
		return-1;
	}
	string lpcommand;
	for (int n=0;n<command.size();n++)
	{
	bool find=false;
		for (int x=0;x<strlen(send_allow);x++)
			if (send_allow[x]==command.at(n))
				{
					find=true;
					break;
				}
	if (find)
		lpcommand+=command.at(n);
	}
	pwrite(io, command.data(), command.size());
	char buff[MAX_BUFF_SIZE];
	memset(buff, 0, MAX_BUFF_SIZE);
	pread(io, buff, MAX_BUFF_SIZE);
	printf("%s", buff);
	return 0;
}
