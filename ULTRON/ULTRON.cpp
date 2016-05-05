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
#include <TAHITI/ExtenisionLIB/configure.h>
#include <TAHITI/ExtenisionLIB/translate.h>

CONFIG cfg;
USER user;
char acode[AUTH_CODE_LEN];
	string config_file = "";

	void for_each_api_fun(const BLOCK info)
	{
		if (!strcmp(info.name.data(), "server"))
		{
			kernel().server = info.value;
			return;
		}
		if (!strcmp(info.name.data(), "port"))
		{
			kernel().port = atoi(info.value.data());
			return;
		}
		if (!strcmp(info.name.data(), "max_try_conn"))
		{
			kernel().max_try_conn = atoi(info.value.data());
			return;
		}
		if (!strcmp(info.name.data(), "try_wait"))
		{
			kernel().try_wait = atoi(info.value.data());
			return;
		}
		if (!strcmp(info.name.data(), "mod_call_wait"))
		{
			kernel().mod_call_wait = atoi(info.value.data());
			return;
		}
		if (!strcmp(info.name.data(),"name"))
		{
			kernel().device_name = info.value;
			return;
		}
	}

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
					kernel().port = atoi(argv[n]+2);
					break;
				case 's':
					kernel().server = argv[n] + 2;
					break;
				case 'n':
					kernel().device_name = argv[n]+2;
					break;
				case 'c':
					config_file = argv[n]+2;
					break;
				case 't':
					load_asci(argv[n]+2);
					break;
				case 'q':
					kernel().quiet = true;
					break;
				case 'u':
					user.name = argv[n]+2;
					break;
				case 'P':
					user.password = argv[n] + 2;
					break;
				default:
					cout << "Use Age:" << argv[0] << " [command] [option]" << endl;
					break;
			}
		}
		if (user.name.size() != 0 || user.password.size() != 0)
			auth_key_create(user);
		if (strcmp(argv[1], "list")==0)
		{
			for (int n = 0; n < get_modules_size(); n++)
				cout << n << ":" << get_mod(n).get_name() << endl;
			return-1;
		}
		if (!config_file.empty())
		{
			init_configure(config_file, cfg, false);
			for_each(for_each_api_fun, cfg, false);
		}
		cout << "Connect To:" << kernel().server << ":" << kernel().port << endl;
		vector<string> cmd;
		for (int n = 2; n < argc; n++)
			if (argv[n][0] != '-')
				cmd.push_back(argv[n]);
		cout << "Start Search Modules Poll" << endl;
		for (long long n = 0; n < get_modules_size(); n++)
		{
			if (!strcmp(get_mod(n).get_name().data(), argv[1]))
			{
				if (get_mod(n).get_cli() == NULL)
				{
					cout << "This method is not support on this modules!" << endl;
					exit(-1);
				}
				cout << "Starting Modules:" << get_mod(n).get_name() << endl;
				return get_mod(n).get_cli()(cmd);
			}
		}
		cout << "unknow command!" << endl << "command:" << argv[1] << endl;
		return 0;
	}


	int sys_exec_data_send(vector<string>spoll)
	{
		ROUTING_INFO rout;
		string cmd;
		for (int n = 1; n < spoll.size(); n++)
		{
			cmd.append(" ");
			cmd.append(spoll.at(n));
		}
		strcpy(rout.name, spoll.at(0).data());
		strcpy(rout.data, cmd.data());
		rout.def = 0x55555555;
		cout << "Sending Data:" << rout.data << " --> " << rout.name << endl;
		DATA_FORMAT send;
		memcpy(send.auth_key, user.auth_key, AUTH_CODE_LEN);
		send.def = 0x995DFA6B;
		memcpy(send.buff, &rout, sizeof(rout));
		send.real_data_len = sizeof(ROUTING_INFO);
		SOCKET conn = create_connect();
		data_send(send, conn);
		DATA_FORMAT last;
		data_recv(conn, last);
		return 0;

	}


	Modules sys_exec_cli("exec", sys_exec_data_send);