// TAHITI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "inline.h"
#include <libSemphella/apd.h>
#include "modules.h"


libDebug tahiti("TAHITI_MAIN");
APD config;

KERNEL ker;
string config_file = "";

int main(int argc, char *argv[])
{
#ifndef __LINUX__
	WSAData wsa;
	WORD word = MAKEWORD(2, 2);
	WSAStartup(word, &wsa);
#endif
	if (argc < 2)
	{
		cout << "Use Age:" << argv[0] << " [command] [option]" << endl;
		return -1;
	}
	for (int n = 0; n < argc; n++)
	{
		if (argv[n][0] == '-')
			switch (argv[n][1])
		{
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
	}
	cout << "Connect To:" << ker.server << ":" << ker.port << endl;
	vector<string> cmd;
	for (int n = 2; n < argc; n++)
		cmd.push_back(argv[n]);
	for (long long n = 0; n < get_modules_size(); n++)
	{
		if (!strcmp(get_mod(n).get_name().data(), argv[1]))
		{
			if (get_mod(n).get_cli() == NULL)
			{
				cout << "This method is not support on this modules!" << endl;
				exit(-1);
			}
			return get_mod(n).get_cli()(cmd);
		}
	}
	cout << "unknow command!" << endl << "command:" << argv[1] << endl;
	return 0;
}

KERNEL kernel()
{
	return ker;
}

int __server_relink(vector<string> command)
{
	server_main();
	return 0;
}

int __client_main(vector<string> command)
{
	client_main();
	return 0;
}

Modules server_link("server", __server_relink);
Modules client_link("client_daemon", __client_main);