#include "stdafx.h"
#include "feature_define.h"
#include "modules.h"
#include "inline.h"
DEVICE GETLOCAL();
SOCKET clientSocket;
libDebug client_dbg("ClientDaemon");
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
				if (get_mod(n).get_client_to() != NULL&&get_mod(n).get_api()!=CLI_FEATURE)
				{
			client_dbg << "=========================" << endl << "Work for:" << get_mod(n).get_name()<< endl;
			DATA_FORMAT push, ret;
			push.def = get_mod(n).get_api();
			get_mod(n).get_client_to()(push);
			ret=network_trans(push);
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

