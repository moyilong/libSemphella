// TAHITI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "inline.h"

bool server_daemon = false;

KERNEL ker;

int main(int argc, char *argv[])
{
#ifndef __LINUX__
	WSAData wsa;
	DWORD word = MAKEWORD(2, 2);
	WSAStartup(word, &wsa);
#endif
	for (int n = 0; n < argc; n++)
	{
		if (argv[n][0] == '-')
			switch (argv[n][1])
		{
			case 'd':
				server_daemon = true;
				break;
			case 'p':
				n++;
				ker.port = atoi(argv[n]);
				break;
			case 's':
				n++;
				ker.server = argv[n];
				break;
			default:
				break;
		}
	}
	if (server_daemon)
		server_main();
	else
		client_main();
	return 0;
}

KERNEL kernel()
{
	return ker;
}