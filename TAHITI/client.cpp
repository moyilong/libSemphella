#include "stdafx.h"
#include "feature_define.h"
#include "modules.h"
#include "inline.h"
#include "ExtenisionLIB/translate.h"
DEVICE GETLOCAL();
SOCKET clientSocket;

#define client_dbg cout<<"[ClientDaemon]"



void client_main()
{
	client_dbg << "Client Daemon is Start!" << endl;
	ENV_DATA __env__temp;
	trigger(CLIENT_DAEMON_INIT, __env__temp);
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(kernel().port) ;
	struct hostent *host;
	host = gethostbyname(kernel().server.data());
	server.sin_addr = *((struct in_addr *)host->h_addr);
	client_dbg << translate("Information is prepared!") << endl;
	client_dbg << translate("Start of ") << get_modules_size() << endl;
	while (true)
	{
		for (int n = 0; n < get_modules_size(); n++)
		{
			if (get_mod(n).get_client_to() != NULL&&get_mod(n).get_api() != CLI_FEATURE)
			{
				client_dbg << "=========================" << endl;
				client_dbg << translate("Work for:") << get_mod(n).get_name() << endl;
				DATA_FORMAT push, ret;
				push.def = get_mod(n).get_api();
				get_mod(n).get_client_to()(push);
				network_trans(push,ret);
				if (ret.def != get_mod(n).get_api())
				{
					for (int b = 0; b < get_modules_size(); b++)
						if (get_mod(b).get_api() == ret.def&&get_mod(b).get_client_ret()!=NULL)
							get_mod(b).get_client_ret()(ret);
				}
				else{
					get_mod(n).get_client_ret()(ret);
				}
				close(clientSocket);
				esleep(kernel().mod_call_wait);
			}
		}
	}

	}

