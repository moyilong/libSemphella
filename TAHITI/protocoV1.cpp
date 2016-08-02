#include "stdafx.h"
#include "inline.h"
#include "feature_define.h"
#include "modules.h"

#define PROTOCO_DEF	cout<<"[Protoco]"

DATA_FORMAT network_trans(DATA_FORMAT to, DATA_FORMAT &ret)
{
	SOCKET conn = create_connect();
	data_send(to, conn);
	data_recv(conn, ret);
	if (ret.def == ERR_BACK)
		if (strcmp(ret.buff, _ERROR_NO_ERROR))
		{
			cout << "Server return a ERROR!!" << endl;
			cout << ret.buff << endl;
			ENV_DATA data;
			data.data = ret;
			trigger(SERVER_ERR_BACK, data);
		}
	close(conn);
	return ret;
}

SOCKET create_connect(int port, string target)
{
	if (port == -1)
		port = kernel().port;
	if (target.empty())
		target = kernel().server;
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	struct hostent *host = gethostbyname(target.data());
	server.sin_addr = *((struct in_addr *)host->h_addr);

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
	{
		trigger(SOCK_FAILD);
		return INVALID_SOCKET;
	}
	int create_count = 0;
	while (connect(client_socket, (sockaddr*)&server, sizeof(server)) == INVALID_SOCKET)
	{
		create_count++;
		if (create_count % 10 == 0)
		{
			DEBUG_LINE cout << "Warring of connect! try " << create_count << "//" << kernel().max_try_conn << endl;
		}
		if (create_count > kernel().max_try_conn)
		{
			cout << "Connect Create Faild!" << endl;
			ENV_DATA data;
			trigger(NETWORK_BREAK, data);
		}
		esleep(kernel().try_wait);
	}
	return client_socket;
}

SOCKET create_connect_server(int port)
{
	if (port == -1)
		port = kernel().port;
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (sListen == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}

	sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sListen, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
		trigger(BIND_FAILD);
		return INVALID_SOCKET;
	}

	if (listen(sListen, 5) == SOCKET_ERROR) {
		cout << "Listen Prot Faild!" << endl;
		trigger(LISTEN_FAILD);
		return INVALID_SOCKET;
	}
	return sListen;
}

void entry_server(server_mode_call_back api, int port)
{
#ifdef __LINUX__
#define __UNSIGNED_INT
#endif

#ifdef __ANDROID__
#undef __UNSIGNED_INT
#endif
	SOCKET server_sock = create_connect_server(port);
	bool ret = true;
	while (ret)
	{
		SOCKET sAccept;
		sockaddr_in client;
#ifdef __UNSIGNED_INT
		unsigned
#endif
			int iLen = sizeof(client);
		sAccept = accept(server_sock, (sockaddr*)&client, &iLen);
		ret = api(sAccept, client);
		close(sAccept);
	}
}

int v1_data_send(DATA_FORMAT data, SOCKET conn)
{
	char buff[SEND_LEN];
	memcpy(buff, &data, SEND_LEN);
	Convert(buff, SEND_LEN);
	send(conn, buff, SEND_LEN, 0);
	return ERR_NO_ERROR;
}

int v1_data_recv(SOCKET conn, DATA_FORMAT &ret)
{
	char get[SEND_LEN];
	recv(conn, get, SEND_LEN, 0);

	Convert(get, SEND_LEN);
	memcpy(&ret, get, SEND_LEN);
#ifdef EXEC_HEAD_CHECK
	if (ret.HEAD_CHECK_B != GetHeadCheck(ret.HEAD_CHECK_A))
	{
		cout << "Head Check Faild!" << endl;
		trigger(RECV_DATA_UNKNOW);
		return ERR_UNKNOW_DATA;
	}
#endif
	if ((ret.API_LEVEL > S_API_VERSION) && (ret.API_LEVEL < S_API_ALLOW_MIN))
	{
		ENV_DATA env;
		env.data = ret;
		trigger(API_MISMATCH, env);
#ifndef ALLOW_API_MISMATCH
		return ERR_API_MISMATCH;
#endif
	}
	if (ret.endia_stat != ENDIAN_TEST())
	{
		cout << "Endian Test Faild!" << endl;
		cout << "Package Endian:" << (int)ret.endia_stat << " != " << (int)ENDIAN_TEST() << endl;
		ENV_DATA env;
		env.data = ret;
		trigger(ENDIAN_ERROR, env);
#ifndef ALLOW_ENDIAN_MISMATCH
		return ERR_API_MISMATCH;
#endif
	}
	ENV_DATA data;
	data.data = ret;
	data.conn = conn;
	trigger(SERVER_BACK, data);
	return ERR_NO_ERROR;
}

char GetHeadCheck(char HDA) {
	return (HDA * 0x2B);
}

int data_send(DATA_FORMAT data, SOCKET conn)
{
	int ret = 0;
	if (!kernel().old_protoco_version)
		ret = v2_send_data(conn, data);
	else
		ret = v1_data_send(data, conn);
	if (ret != 0)
	{
		cout << "Protoco Return An Error!" << endl;
	}
	return ret;
}

int data_recv(SOCKET conn, DATA_FORMAT &ret)
{
	int xret = 0;
	if (!kernel().old_protoco_version)
		xret = v2_recv_data(conn, ret);
	else
		xret = v1_data_recv(conn, ret);
	if (xret != 0)
	{
		cout << "Protoco Return An Error!" << endl;
	}
	return xret;
}

uint16_t fast_server_online_test()
{
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(kernel().port);
	struct hostent *host = gethostbyname(kernel().server.data());
	if (host == nullptr)
	{
		return -1;
	}
	server.sin_addr = *((struct in_addr *)host->h_addr);

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
		return -1;
	int create_count = 0;
	clock_t tout = clock();
	if (connect(client_socket, (sockaddr*)&server, sizeof(server)) == INVALID_SOCKET)
		return -1;
	tout = clock() - tout;
	close(client_socket);
	return tout;
}