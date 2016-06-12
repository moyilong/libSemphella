#include "modules.h"
vector<modules> modpoll;
modules::modules(WORK_CMD _cmd, server_api _api)
{
	cmd = _cmd;
	api = _api;
	modpoll.push_back(*this);
	DEBUG_LINE printf("Load Modules:%ull\n", _cmd);
}

int max_try_conn = 3;
int try_wait = 1000;
SOCKET create_connect(int port, string target)
{
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	struct hostent *host = gethostbyname(target.data());
	server.sin_addr = *((struct in_addr *)host->h_addr);

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
	{
		debug << "Create Socket Faild!" << endl;
		return INVALID_SOCKET;
	}
	int create_count = 0;
	while (connect(client_socket, (sockaddr*)&server, sizeof(server)) == INVALID_SOCKET)
	{
		create_count++;
		if (create_count % 10 == 0)
		{
			DEBUG_LINE cout << "Warring of connect! try " << create_count << "//" << max_try_conn << endl;
		}
		if (create_count > max_try_conn)
		{
			cout << "Connect Create Faild!" << endl;
			throw CONN_FAILD;
		}
		esleep(try_wait);
	}
	return client_socket;
}

SOCKET create_connect_server(int port)
{
	debug << "Creat Srever Listen on port:" << port << endl;
	sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (sListen == INVALID_SOCKET) {
		debug << "Create Socket Faild!" << endl;
		return INVALID_SOCKET;
	}


	if (bind(sListen, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
		debug << "Bind Socket Faild!" << endl;
		return INVALID_SOCKET;
	}

	if (listen(sListen, 5) == SOCKET_ERROR) {
		cout << "Listen Prot Faild!" << endl;
		return INVALID_SOCKET;
	}
	return sListen;
}
