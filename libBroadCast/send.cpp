// libBroadCast.cpp : 定义 DLL 应用程序的导出函数。
//

#include "libBroadCast.h"
#include <libSemphella/string.h>
#include <libSemphella/main.h>
#include <libSemphella/net.h>

BC_API void SendBroadcast(int NET_PORT,uint64_t second, ProcdConfigure api)
{
	int connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (connect_socket == SOCKET_ERROR)
	{
		debug << "Create Socket Faild!" << endl;
		return;
	}
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(NET_PORT);
	sin.sin_addr.s_addr = 0;

	sockaddr_in sin_form;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(NET_PORT);
	sin.sin_addr.s_addr = INADDR_BROADCAST;

	bool bOpt = true;
	int err = setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
	if (err == SOCKET_ERROR)
	{
		debug << "Set SocketOPT Faild!" << endl;
		return;
	}
	err = bind(connect_socket, (sockaddr *)&sin, sizeof(sockaddr));
	if (err == SOCKET_ERROR)
	{
		debug << "Bind Socket Error!" << endl;
		return;
	}
	char *buffers;
	int len;
	api.hs(buffers, len);
	err = sendto(connect_socket, buffers, len, 0, (sockaddr *)&sin, sizeof(sin));
	if (err == SOCKET_ERROR)
	{
		debug << "Send Head Faild!" << endl;
		return;
	}
	time_t begin = time(0);
	char recv_buff[RECV_BUFF];
	while (true)
	{
		if (time(0) - begin > second)
			break;
		int pg = sizeof(sin);
		err = recvfrom(connect_socket, recv_buff, RECV_BUFF, 0, (sockaddr*)&sin_form, &pg);
		if (err == SOCKET_ERROR)
		{
			debug << "Socket Read an error package!" << endl;
			continue;
		}
		api.recv(recv_buff);
	}
}