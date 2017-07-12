// libBroadCast.cpp : 定义 DLL 应用程序的导出函数。
//

#include "libBroadCast.h"
#include <libSemphella/string.h>
#include <libSemphella/main.h>
#include <libSemphella/net.h>


BC_API void StartListenDaemon(int NET_PORT,ProcdConfigure api)
{
	SOCKET connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (connect_socket == INVALID_SOCKET)
	{
		debug << "Socket Error!" << endl;
		return;
	}
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(NET_PORT);
	sin.sin_addr.s_addr = INADDR_BROADCAST;

	bool bOpt = true;
	setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
	char buff[RECV_BUFF];
	int err = 0;
	while (true)
	{
		int pg = sizeof(sin);
		err = recvfrom(connect_socket, buff, RECV_BUFF, 0, (sockaddr*)&sin, &pg);
		if (SOCKET_ERROR == err)
		{
			debug << "Recv an Error Package!" << endl;
			continue;
		}
		char *buffers;
		int len;
		api.hp(buff, buffers, len);
		sendto(connect_socket, buffers, len, 0, (sockaddr*)&sin, pg);

	}
}