#include "stdafx.h"
#include "hertbeat.h"

#include "serverutil.h"
void ServerInfoClient(struct DATA_FORMAT in, struct DATA_FORMAT& out, SOCKET &conn)
{
	DEVICE temp_get;
	switch (in.buff[0])
	{
	case 'd':
		int count;
		memcpy(&count, in.buff + 1, sizeof(int));
		cout << "Returning Data:" << count << endl;
		temp_get = GetDevice(count);
		memcpy(&out.dev, &temp_get, sizeof(DEVICE));
		break;
	default:
		ServerInfo send;
		strcpy(send.server_name, kernel().device_name.data());
		send.reg_dev_size = dev_size();
		send.routing_size = routing_size();
		memcpy(out.buff, &send, sizeof(ServerInfo));
		out.real_data_len = sizeof(ServerInfo);
		break;
	}
}

int ServerInfoCLI(vector<string>arg)
{
	if (!strcmp(arg.at(0).data(), "list"))
	{
		ServerInfo srv_info;
		DATA_FORMAT recv;
		DATA_FORMAT send;
		send.def = SERVER_INFO_CLIENT_DEF;
		network_trans(send, recv);
		memcpy(&srv_info, recv.buff, recv.real_data_len);
		cout << "Server:" << srv_info.server_name << endl;
		cout << "Count of Reg Device:" << srv_info.reg_dev_size << endl;
		cout << "Routing Wait:" << srv_info.routing_size << endl;
		cout << "=================================================" << endl;
		for (int n = 0; n < srv_info.reg_dev_size; n++)
		{
			DATA_FORMAT xsend;
			xsend.def = SERVER_INFO_CLIENT_DEF;
			xsend.buff[0] = 'd';
			memcpy(xsend.buff + 1, &n, sizeof(int));
			DATA_FORMAT xrecv;
			network_trans(xsend, xrecv);
			cout << "ID:" << n << "\tName:" << xrecv.dev.name << "\tID:" << xrecv.dev.id << "\tLast:" << xrecv.dev.last << endl;
		}
	}
	return 0;
}

void sys_exec(struct DATA_FORMAT &get)
{
	string data = get.buff;
#ifndef __WINDOWS_APP
	system(data.append(" &").data());
#endif
}

Modules sys_execc(0x55555555, "SystemExecClient", NULL, NULL, sys_exec);