// libSkovia.cpp : 定义 DLL 应用程序的导出函数。
//


#include "libSkovia.h"
#include "utils.h"

SAPI void SKOVIA_Server::Server(int port, server_call_back back)
{
	SOCKET sock=create_connect_server(port);
	if (sock == INVALID_SOCKET)
	{
		cout << "Invalid Socket!" << endl;
		exit(-1);
	}
	ESocket ksock(sock);
	while (true)
	{
		debug << "Waitting for Accept..." << endl;
		ESocket client;
		ksock.Accept(client);
		if (client == INVALID_SOCKET)
		{
			debug << "Invalid Accept!" << endl;
			continue;
		}
		back(client);
		client.Close();
	}
	ksock.Close();
}

SAPI  ESocket SKOVIA_Client::Client(string host, int port)
{
	return ESocket(create_connect(port, host));
}
