#pragma once
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBNETUTILS_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBNETUTILS_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef __LINUX__
#define LIBNETUTILS_API 
#else
#ifdef LIBNETUTILS_EXPORTS
#define LIBNETUTILS_API __declspec(dllexport)
#else
#define LIBNETUTILS_API __declspec(dllimport)
#endif
#endif

#define NETAPI	LIBNETUTILS_API


#include <libSemphella/main.h>
#include <libSemphella/libSemphella.h>
#include <libSemphella/string.h>
#include <libSemphella/crypt.h>

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

#include <stdint.h>
#include <inttypes.h>

#ifdef __LINUX__
#define esleep(ms) usleep(ms*1000)
#else
#define esleep(ms) Sleep(ms)
#endif

#ifndef __LINUX__
/*inline void close(SOCKET conn)
{
closesocket(conn);
}*/
#define close(conn) closesocket(conn)
#endif

enum throw_t {
	MD_VERIFY_ERROR,
	CREATE_SOCKET_FAILD,
	CONN_FAILD,
};


enum WORK_CMD {
	DOWNLOAD_TEST,
	UPLOAD_TEST,
	TIMEOUT_TEST,
	INFO_GET,
	NULL_RET,
	CMD_RET,
	ERR_RET,
};

NETAPI void service_main(int port);
struct ServerInfo {
	char api_ver;
	uint64_t uptime;
	uint64_t cmd_size;
	uint64_t err_size;
	uint64_t mod_size;
};

//FEATURE_EXPORT
NETAPI ServerInfo ServerInfoGet(string targ_server, int port);
NETAPI double Download(string targsrv, int block_size, int port);
NETAPI double Upload(string targsrv, int block_size, int port);