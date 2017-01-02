#pragma once
#include "debug.h"
#include "string.h"
#include "error.h"
#include "crypt.h"
#ifndef __linux__
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define closesocket(socket) close(socket)
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#define SOCKET int

#endif

#define SP_NET_API	(float)1.00

struct nconfig {
	int max_try = 4;
	double try_to_try_wait = 1000;
};


API bool SendEmail(const string& smtpServer, const string& username, const string& pw, const string& to, const string& data);