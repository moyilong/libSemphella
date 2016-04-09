// DragonEDGE.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#ifdef __LINUX__
#define PORT	"/dev/ttyUSB0"
#else
#define PORT	"COM3"
#endif

const char send_allow[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";
#define READSIZE	512
string port = PORT;
string command = "";
int arg_stop = 0;
#include "api.h"
const DEVICE_STAT lpstat;
int main(int argc, char *argv[])
{
	for (int n = 1; n < argc; n++)
		if (argv[n][0] == '-')
			switch (argv[n][1])
			{
			case 'p':
			case 'P':
				n++;
				port = argv[n];
				break;
			case 'c':
			case 'C':
				arg_stop = n + 1;
				break;
			default:
				cout << "Unknow Command:" << argv[n] << endl;
				break;
			}
	IO_TYPE io = pinit(port, lpstat);
	if (io == INVALID_HANDLE_VALUE || io == NULL)
	{
		cout << "Get IO Operate Fiald!" << endl;
		return-1;
	}
	for (int n = arg_stop; n < argc; n++)
	{
		command += argv[n];
		command += " ";
	}
	string lpcommand;
	for (int n = 0; n < command.size(); n++)
	{
		bool find = false;
		for (int x = 0; x < strlen(send_allow); x++)
			if (send_allow[x] == command.at(n))
			{
				find = true;
				break;
			}
		if (find)
			lpcommand += command.at(n);
	}
	command += "\\";
	pwrite(io, command.data(), command.size());
	string rbuff;
	char buff[READSIZE];
	memset(buff, 0, READSIZE);
	//pread(io, buff, READSIZE);
	//printf("%s", buff);
	for (int n = 0; n < READSIZE; n++)
	{
		pread(io, buff + n, 1);
		cout << "Read Str:" << buff[n] << endl;
		if (buff[n] == '\\')
		{
			buff[n] = '\0';
			break;
		}
	}
	cout << buff << endl;
	return 0;
}