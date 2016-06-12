#include "libNetUtils.h"
#include "protoco.h"
#include "modules.h"
uint64_t err_size=0;
uint64_t beg_time=0;
uint64_t  cmd_size=0;
NETAPI void service_main(int port)
{
	beg_time = time(0);
	SOCKET sock = create_connect_server(port);
	if (sock == INVALID_SOCKET)
		throw CREATE_SOCKET_FAILD;
	while (true)
	{
		sockaddr_in addr;
#ifdef __LINUX__
		unsigned
#endif
			int klen = sizeof(addr);
		SOCKET acc = accept(sock, (sockaddr*)&addr, &klen);
		char *ptr = nullptr;
		uint64_t len = 0;
		WORK_CMD cmd = PortGet(acc, ptr, len);
		void *to_ptr = NULL;
		uint64_t to_len = 0;
		cmd_size++;
		if (cmd != NULL_RET)
		{
			for (int n = 0; n < modpoll.size(); n++)
				if (cmd == modpoll.at(n).cmd)
					modpoll.at(n).api(acc, ptr, len, to_ptr, to_len);
		}
		else {
			err_size++;
		}
		PortSend(acc, to_ptr, to_len, CMD_RET);
		close(acc);
	}
}


void ServerInfoServer(API_ARGS_DEF)
{
	ServerInfo info;
	info.api_ver = HEAD_VER;
	info.err_size = err_size;
	info.cmd_size = cmd_size;
	info.mod_size = modpoll.size();
	info.uptime = time(0) - beg_time;
	send_ptr = &info;
	send_len = sizeof(ServerInfo);
}

MOD ServerInfoRegister(INFO_GET, ServerInfoServer);

NETAPI ServerInfo ServerInfoGet(string targ_server,int port)
{
	SOCKET sock=create_connect(port, targ_server);
	if (sock == INVALID_SOCKET)
		throw CREATE_SOCKET_FAILD;
	PortSend(sock, NULL, 0, INFO_GET);
	ServerInfo ret;
	uint64_t len;
	PortGet(sock, &ret, len);
	close(sock);
	return ret;
}