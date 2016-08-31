#include "libSkovia.h"
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
		cout << "Create Socket Faild!" << endl;
		debug << "Create Socket Faild!" << endl;
		return INVALID_SOCKET;
	}
	int create_count = 0;
	while (connect(client_socket, (sockaddr*)&server, sizeof(server)) == INVALID_SOCKET)
	{
		create_count++;
		if (create_count % 10 == 0)
		{
			cout << "Warring of connect! try " << create_count << "//" << 4 << endl;
		}
		if (create_count > 4)
		{
			cout << "Connect Create Faild!" << endl;
			return INVALID_SOCKET;
		}
		esleep(500);
	}
	return client_socket;
}

SOCKET create_connect_server(int port)
{
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (sListen == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}

	sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sListen, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
		return INVALID_SOCKET;
	}

	if (listen(sListen, 5) == SOCKET_ERROR) {
		cout << "Listen Prot Faild!" << endl;
		return INVALID_SOCKET;
	}
	return sListen;
}