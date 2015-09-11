#include "include.h"
#include "feature.h"
#include "utils.h"
#include "mod.h"
libDebug server("Server");
int protoco=0;

void server_main()
{
	server << "========= Insight Server =========" << endl;
	server << "Feature API Version:" << FEATURE_API << endl;
	server << "Compact API Version:" << COMPACT_API << endl;
	server << "===================================" << endl;
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (sListen == INVALID_SOCKET)
	{
		server << "Create Socket Port Faild!" << endl;
		exit(-1);
	}
	sockaddr_in srv;
	srv.sin_family = AF_INET;
	srv.sin_port = htons(DEFAULT_PORTS);
	srv.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sListen, (sockaddr*)&srv, sizeof(srv)) == SOCKET_ERROR)
	{
		server << "Bind To port Faild!" << endl;
		exit(-1);
	}

	if (listen(sListen, 5) == SOCKET_ERROR)
	{
		server << "Socket Listen Error!" << endl;
		exit(-1);
	}

	while (true)
	{
		server << "Waitting for Accept..." << endl;
		SOCKET Accept=INVALID_SOCKET;
		sockaddr_in addr;
		int len = sizeof(sockaddr_in);
		Accept = accept(sListen, (sockaddr*)&addr,&len);
		char buff[HEAD_LEN] = { 0x00 };
		recv(Accept, buff, HEAD_LEN, 0);
		bool stat = true;
		CONN_HEAD ret = dePackage(buff,stat);
		if (!stat)
		{
#ifndef __LINUX__
			closesocket(Accept);
#else
			close(Accept);
#endif
		}
		CONN_HEAD back;
		back.type = APICALL_NOTHING;
		back.protoco = protoco;
		modules_api api = get_modapi(ret.type);
		if (api == NULL)
			back.type = APICALL_ERROR;
		else
			if (!api(ret, back))
			{
			server << "Modules Report a Error!" << endl;
			}
		char sbuff[HEAD_LEN] = { 0x00 };
		Package(back, sbuff);
		send(Accept, sbuff, HEAD_LEN, 0);
#ifndef __LINUX__
		closesocket(Accept);
#else
		close(Accept);
#endif
	}
#ifndef __LINUX__
	closesocket(sListen);
#else
	close(sListen);
#endif
}

bool switch_protoco(const CONN_HEAD getd, CONN_HEAD&ret)
{
	protoco = getd.protoco;
	return true;
}

mod __switch_protoco(SWITCH_PROTOCO, switch_protoco);