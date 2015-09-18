// TAHITI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "inline.h"
#include <libSemphella/apd.h>
#include "modules.h"


libDebug tahiti("TAHITI_MAIN");


KERNEL ker;
bool first_init = false;

KERNEL &kernel()
{
	if (!first_init)
	{
#ifndef __LINUX__

		WSAData wsa;
		WORD word = MAKEWORD(2, 2);
		WSAStartup(word, &wsa);
#endif
		first_init = true;
		
	}
	return ker;
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

Modules server_link("server", __server_relink);
Modules client_link("client_daemon", __client_main);