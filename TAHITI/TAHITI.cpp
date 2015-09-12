// TAHITI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "inline.h"

bool server_daemon = false;

#include <libSemphella/apd.h>

using namespace APD_UTILS;

APD config;

KERNEL ker;
string config_file = "";

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
			case 'n':
				n++;
				ker.device_name = argv[n];
				break;
			case 'c':
				n++;
				config_file = argv[n];
				break;
			default:
				break;
		}
	}
	if (!config_file.empty())
	{
		config.load(config_file);
		if (config.check_label("main", "server") != -1)
			ker.server = config.get_label("main", "server");
		if (config.check("main", "port") != -1)
			ker.port = atoi(config.get_label("main", "port").data());
		if (config.check("main", "name") != -1)
			ker.device_name = config.get_label("main", "name");
		if (config.check("main", "sleep") != -1)
			ker.sleep_time = atoi(config.get_label("main", "sleep").data());
	}
	cout << "Connect To:" << ker.server << ":" << ker.port << endl;
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