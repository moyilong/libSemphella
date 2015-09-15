#include "stdafx.h"
#include "inline.h"
#include "feature_define.h"
#include "modules.h"

#ifndef __LINUX__
inline void close(SOCKET conn)
{
	closesocket(conn);
}
#endif

DATA_FORMAT network_trans(DATA_FORMAT to)
{
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(kernel().port);
	struct hostent *host = gethostbyname(kernel().server.data());
	server.sin_addr = *((struct in_addr *)host->h_addr);

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	char  push[SEND_LEN] = { 0x00 };
	char pull[SEND_LEN] = { 0x00 };
	int create_count = 0;
	while (connect(client_socket, (sockaddr*)&server, sizeof(server)) == INVALID_SOCKET)
	{
		create_count++;
		if (create_count % 10 == 0)
		{
			DEBUG_LINE cout << "Warring of connect! try "<<create_count << endl;
		}
		if (create_count > 1000)
		{
			cout << "Connect Create Faild!" << endl;
			exit(-1);
		}
		esleep(500);
	}
	memcpy(push, &to, SEND_LEN);
	Convert(push, SEND_LEN);
	send(client_socket, push, SEND_LEN, 0);
	recv(client_socket, pull, SEND_LEN, 0);
	Convert(pull, SEND_LEN);
	DATA_FORMAT ret;
	memcpy(&ret, pull,SEND_LEN);
	if (ret.def == ERR_BACK)
		if (strcmp(ret.buff, _ERROR_NO_ERROR))
		{
		cout << "Server return a ERROR!!" << endl;
		cout << ret.buff << endl;
		}
	close(client_socket);
	return ret;


}