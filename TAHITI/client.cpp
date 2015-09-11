#include "stdafx.h"
#include "feature_define.h"
#include "modules.h"
#include "inline.h"
DEVICE GETLOCAL();
SOCKET clientSocket;

#ifndef __LINUX__
inline void close(SOCKET conn)
{
	closesocket(conn);
}
#endif

void client_main()
{

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(kernel().port) ;
	struct hostent *host;
	host = gethostbyname(kernel().server.data());
	server.sin_addr = *((struct in_addr *)host->h_addr);

		while (true)
			for (int n = 0; n < get_modules_size(); n++)
				if (get_mod(n).get_client_to() != NULL)
				{
			DEBUG_LINE cout << "=========================" << endl << "Work for:" << get_mod(n).get_name()<< endl;
			DEBUG_LINE cout << "create connect!" << endl;
			clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			int create_count = 0;
			while (connect(clientSocket, (sockaddr*)&server, sizeof(server)) == INVALID_SOCKET)
			{
				create_count++;
				if (create_count % 10==0)
				{
					DEBUG_LINE cout << "Warring of Create Connect!  Try:"<<create_count << endl;
				}
				esleep(100);
			}
			DATA_FORMAT to;
			DATA_FORMAT ret;
			to.def = get_mod(n).get_api();
			memcpy(&to.dev, &GETLOCAL(), sizeof(DEVICE));
			get_mod(n).get_client_to()(to);
			char buff[SEND_LEN] = { 0x00 };
			memcpy(buff, &to, SEND_LEN);
			Convert(buff, SEND_LEN);
			DEBUG_LINE cout << "Sending Data!" << endl;
			send(clientSocket, buff, SEND_LEN, 0);
			char rbuff[SEND_LEN] = { 0x00 };
			DEBUG_LINE cout << "Waiting for Server Back!" << endl;
			recv(clientSocket, rbuff, SEND_LEN, 0);
			Convert(rbuff, SEND_LEN);
			memcpy(&ret, rbuff, SEND_LEN);
			DEBUG_LINE cout << "return MATCH:" << ret.def << endl;
			if (ret.def == ERR_BACK)
				if (strcmp(ret.buff, _ERROR_NO_ERROR))
				{
				cout << "Server return a ERROR!!" << endl;
				cout << ret.buff << endl;
				}
			if (ret.def != get_mod(n).get_api())
			{
				for (int b = 0; b < get_modules_size(); b++)
					if (get_mod(b).get_api() == ret.def)
						get_mod(b).get_client_ret()(ret);
			}
			else{
				get_mod(n).get_client_ret()(ret);
			}
			close(clientSocket);
			esleep(1000);
				}

	}

