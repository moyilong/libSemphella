#pragma once
#include <inttypes.h>
#ifndef __LINUX__

#ifdef LIBBROADCAST_EXPORTS
#define BC_API	__declspec(dllexport)
#else
#define BC_API __declspec(dllimport)
#endif

#else

#define BC_API

#endif

#define RECV_BUFF	8192

typedef void(*RecvData)(const char *ptr);
typedef void(*HeadSend)(char *ptr,int &len);
typedef void(*HeadProc)(const char *ptr,char *gptr,int &len);

struct ProcdConfigure {
	RecvData recv;
	HeadSend hs;
	HeadProc hp;
};

BC_API void SendBroadcast(int NET_PORT, uint64_t second, ProcdConfigure api);
BC_API void StartListenDaemon(int NET_PORT, ProcdConfigure api);