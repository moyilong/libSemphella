// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBSKOVIA_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBSKOVIA_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef __LINUX__

#ifdef LIBSKOVIA_EXPORTS
#define LIBSKOVIA_API __declspec(dllexport)
#else
#define LIBSKOVIA_API __declspec(dllimport)
#endif

#else
#define LIBSKOVIA_API
#endif

#define SAPI	LIBSKOVIA_API
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
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#endif

#ifdef __LINUX__
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
typedef unsigned int SOCKET;
#endif
#include <libSemphella/main.h>
#include <libSemphella/debug.h>
#include "socket.h"
namespace SKOVIA_Server {
	typedef void(server_call_back)(ESocket sock);
	SAPI  void Server(int port, server_call_back back);
}

namespace SKOVIA_Client {
	SAPI  ESocket Client(string host, int port);
}