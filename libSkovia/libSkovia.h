// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LIBSKOVIA_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LIBSKOVIA_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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