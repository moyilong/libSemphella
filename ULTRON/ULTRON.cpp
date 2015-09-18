// ULTRON.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include <TAHITI/net_base.h>
#include <TAHITI/modules.h>
#include <TAHITI/inline.h>

#include <libSemphella/libsemphella.h>
#include <libSemphella/apd.h>

APD config;


	string config_file = "";

	int main(int argc, char *argv[])
	{

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
					kernel().port = atoi(argv[n]);
					break;
				case 's':
					n++;
					kernel().server = argv[n];
					break;
				case 'n':
					n++;
					kernel().device_name = argv[n];
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
				kernel().server = config.get_label("main", "server");
			if (config.check("main", "port") != -1)
				kernel().port = atoi(config.get_label("main", "port").data());
			if (config.check("main", "name") != -1)
				kernel().device_name = config.get_label("main", "name");
		}
		cout << "Connect To:" << kernel().server << ":" << kernel().port << endl;
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