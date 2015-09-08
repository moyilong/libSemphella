#include "include.h"
#include "feature.h"

WORD dword_type;
WSADATA wsaDATA;

libDebug utils("UTILS");

void init()
{
#ifndef __LINUX__
	dword_type = MAKEWORD(1, 1);
	WSAStartup(dword_type, &wsaDATA);
#endif
}

CONN_HEAD dePackage(char *buff)
{
	crypt(buff, HEAD_LEN, DEFAULT_PASSWORD);
	CONN_HEAD ret;
	memcpy(&ret, buff, HEAD_LEN);
	if (ret.C_API != COMPACT_API)
	{
		utils << "Warring:CompactAPI is mismatch!" << endl;
		exit(-1);
	}
	return ret;
}

void Package(const CONN_HEAD head, char *buff)
{
	memcpy(buff, &head, HEAD_LEN);
	crypt(buff, HEAD_LEN, DEFAULT_PASSWORD);
}