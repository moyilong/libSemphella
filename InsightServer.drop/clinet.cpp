#include "include.h"
#include "mod.h"
#include "feature.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef __LINUX__
#include <netdb.h>
#include <netinet/in.h>
#endif
libDebug dbg("Daemon-Client");
#include "utils.h"
int proto_type=0;
void client_main()
{
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(DEFAULT_PORTS);
	server.sin_addr = *((struct in_addr *)gethostbyname("localhost")->h_addr);


	while (true)
	{
		SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
		if (client == INVALID_SOCKET)
		{
			dbg << "Create Socket Faild!" << endl;
			continue;
		}
		if (connect(client, (sockaddr*)&server, sizeof(server)) == INVALID_SOCKET)
		{
			dbg << "Create Connect Faild!" << endl;
			continue;
		}
		CONN_HEAD send_to;
		send_to.type = APICALL_NOTHING;
		char send_buff[HEAD_LEN] = { 0x00 };
		Package(send_to, send_buff);
		send(client, send_buff, HEAD_LEN, 0);
		char recv_buff[HEAD_LEN] = { 0x00 };
		recv(client, recv_buff, HEAD_LEN, 0);
		bool stat = true;
		CONN_HEAD ret=dePackage(recv_buff, stat);
		if (!stat)
		{
			dbg << "server report a unknow package!" << endl;
			goto disconn;
		}
		if (ret.protoco != proto_type)
		{
			dbg << "Exec ProtoType:" << proto_type << endl;
			string address;
			string exec_command;
#ifdef __LINUX__
			exec_command = "sh "
			address = "/etc/protoco.d";
#else
			exec_command = "cmd.exe /C ";
			address = "%CD%\\protoco.d";
#endif
			char temp[MAX_BUFF_SIZE] = { 0x00 };
			sprintf(temp,"protoco_%d.script", ret.protoco);
			string final_exec = exec_command + address + "/" + temp;
			system(final_exec.data());
		}


	disconn:
#ifdef __LINUX__
		close(client);
#else
		closesocket(client);
#endif
		Sleep(5000);
	}
}