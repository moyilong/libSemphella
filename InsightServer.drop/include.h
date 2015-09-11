#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <libSemphella/libSemphella.h>
#include <libSemphella/crypt.h>

#include <libSemphella/debug.h>


#ifdef __LINUX__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#else
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#endif