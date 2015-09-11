// TAHITI.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "inline.h"

bool server_daemon = false;

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
