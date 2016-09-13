#include"feature.h"
#include <libSemphella/argment.h>

int port=552;
string host="127.0.0.1";
string path;

string project_name="unknow";
string message="unknow";
enum work_mode {
	SERVER,
	SOURCE_CLIENT,
	MGR_CLIENT,
};
work_mode mode = SERVER;
void proc(string name, string val)
{
	switch (name.at(0))
	{
	case 'p':
		port = atoi(val.data());
		break;
	case 's':
		host = val;
		break;
	case 'f':
		path = val;
		break;
	case 'm':
		switch (name.at(1))
		{
		case 's':
			mode = SERVER;
			break;
		case'c':
			mode = SOURCE_CLIENT;
			break;
		case'm':
			mode = MGR_CLIENT;
			break;
		}
		break;
	case '-':
		switch (name.at(1))
		{
		case'p':
			project_name = val;
			break;
		case 'm':
			message = val;
			break;
		}
		break;
	}
}

db md;

void clinet_mgr();
void client_src();
void server();

#include <TAHITI/net_base.h>
#include <TAHITI/multi_platform.h>

int main(int argc, char *argv[])
{
#ifndef __LINUX__
	WSAData wsa;
	WORD word = MAKEWORD(1, 1);
	WSAStartup(word, &wsa);		 
#endif
	argment args;
	args.load(argc, argv);
	args.for_each(proc);
	md.load(path);
	switch (mode)
	{
	case SERVER:
		server();
		break;
	case MGR_CLIENT:
		clinet_mgr();
		break;
	case SOURCE_CLIENT:
		client_src();
		break;
	}
	return 0;
}

void client_src()
{
	request req;
	strcpy(req.message, message.data());
	strcpy(req.project_name, project_name.data());
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "Create Socket Fiald!" << endl;
		return;
	}
	sockaddr_in addr;
	struct hostent *thost = gethostbyname(host.data());
	addr.sin_addr = *((struct in_addr *)thost->h_addr);
	addr.sin_port = port;
	addr.sin_family = AF_INET;
	int try_count = 0;
	while (connect(sock, (sockaddr*)&addr, sizeof(sockaddr_in)) == INVALID_SOCKET)
	{
		try_count++;
		if (try_count == 10)
		{
			cout << "Connect to Server Faild!" << endl;
			return;
		}
		cout << "Waiting for Retry..." << endl;
		esleep(500);
	}
	send(sock, (char*)&req, sizeof(request), 0);
	processed get;
	char buff[sizeof(processed)];
	recv(sock, buff, sizeof(processed), 0);
	close(sock);
	memcpy(&get, buff, sizeof(processed));
	string uid;
	for (int n = 0; n < UID_LEN; n++)
	{
		char buff[16];
		sprintf(buff, "%d", (unsigned int)get.uid[n]);
		uid += buff;
	}
	cout << "UID=" << uid << endl << "BID=" << get.id << endl;
	return;
}

void server()
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in  addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (sock == INVALID_SOCKET)
	{
		cout << "Create Socket Faild!\nChecked System?" << endl;
		return;
	}
	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "Bind Socket Faild!" << endl;
		return;
	}
	if (listen(sock, 10) == INVALID_SOCKET)
	{
		cout << "Listen Port Faild!" << endl;
		return;
	}
	while (true)
	{
		sockaddr_in addr_in;
		int addr_len = sizeof(sockaddr_in);
		SOCKET acct = accept(sock, (sockaddr*)&addr_in, &addr_len);
		char buff[sizeof(request)];
		recv(acct, buff, sizeof(request), 0);
		request req;
		memcpy(&req, buff, sizeof(request));
		build_log log = md.add_log(req.project_name, req.message);
		processed ret;
		ret.id = log.id;
		memcpy(ret.uid, log.uid, UID_LEN);
		send(acct, (char*)&ret, sizeof(processed),0);
		close(acct);
	}
}
