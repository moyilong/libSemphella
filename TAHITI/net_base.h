#ifdef __LINUX__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#else
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#endif


#ifdef __LINUX__
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
typedef unsigned int SOCKET;
#endif