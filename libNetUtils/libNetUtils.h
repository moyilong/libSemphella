#pragma once
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LIBNETUTILS_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LIBNETUTILS_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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