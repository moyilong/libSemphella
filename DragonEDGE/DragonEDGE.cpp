// DragonEDGE.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#ifdef __LINUX__
#define PORT	"tty0"
#else
#define PORT	"COM3"
#endif

string port = PORT;
string command = "";

#include "api.h"
const DEVICE_STAT stat;
int main(int argc, char *argv[])
{
	for (int n = 1; n < argc; n++)
		if (argv[n][0] == '-')
			switch (argv[n][1])
		{
			case 'p':
				port = argv[n] + 2;
				break;
			case 'c':
				n++;
				command = argv[n] ;
				break;
			default:
				cout << "Unknow Command:" << argv[n] << endl;
				break;
		}
	IO_TYPE io=init(port,stat);
	if (io == INVALID_HANDLE_VALUE || io == NULL)
	{
		cout << "Get IO Operate Fiald!" << endl;
		return-1;
	}
	write(io, command.data(), command.size());
	char buff[MAX_BUFF_SIZE];
	memset(buff, 0, MAX_BUFF_SIZE);
	read(io, buff, MAX_BUFF_SIZE);
	printf("%s", buff);
	return 0;
}