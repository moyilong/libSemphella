// TAHITI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "inline.h"
#include "modules.h"

#define tahiti cout<<"[TAHITI_MAIN]"

XKERNEL kernel;
bool first_init = false;

XKERNEL::XKERNEL()
{
	if (first_init)
	{
		cout << "Warring:Multi Init Kernel!" << endl;
		abort();
	}
#ifndef __LINUX__
	WSAData wsa;
	WORD word = MAKEWORD(2, 2);
	WSAStartup(word, &wsa);
#endif
	USER admin;
	admin.name = "moyilong";
	admin.password = "semphella:123";
	admin.leve = ROOT;
	create_user(admin);
	first_init = true;
	kernel_mode = "Compact-Windows";
#ifdef __LINUX__
	kernel_mode = "Compact Linux";
#endif
#ifdef __ANDROID__
	kernel_mode = "Android-Linux";
#endif
#ifdef __WINDOWS_APP
	kernel_mode = "Windows-Application";
#endif
	tahiti << "TAHITI Kernel Starting at " << kernel_mode << " Mode" << endl;
}

int __server_relink(vector<string> command)
{
	server_main();
	return 0;
}

int __client_main(vector<string> command)
{
	client_main();
	return 0;
}
#include "hertbeat.h"
#include "serverutil.h"
Modules server_link("server", __server_relink);
Modules client_link("client_daemon", __client_main);
Modules Hert_beat_link(0xFFFF0001, "HertBeat", Hert_beat_server, Hert_beat_client_to, Hert_beat_client_ret, BLACK_OUT);
Modules RoutingServerX(0x995DFA6B, "RoutingServer", Data_Routing_Server, NULL, NULL, ADMIN);
Modules ServerUtilCli(SERVER_INFO_CLIENT_DEF, "ServerInfo", ServerInfoClient, NULL, NULL, GUEST, ServerInfoCLI, NULL);

char ENDIAN_CACHE = -1;

char ENDIAN_TEST()
{
	if (ENDIAN_CACHE != -1)
	{
		return ENDIAN_CACHE;
	}
	union
	{
		int a;
		char b;
	}c;
	c.a = 1;
	if (c.b == 1)
		ENDIAN_CACHE = LIT_ENDIAN;
	else
		ENDIAN_CACHE = BIG_ENDIAN;
	return ENDIAN_CACHE;
}