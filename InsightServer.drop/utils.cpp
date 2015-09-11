#include "include.h"
#include "feature.h"
#include <libSemphella/string.h>
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

CONN_HEAD dePackage(char *buff,bool &stat)
{
	//crypt(buff, HEAD_LEN, DEFAULT_PASSWORD);
	CONN_HEAD ret;
	display_dump(buff, HEAD_LEN);
	memcpy(&ret, buff, HEAD_LEN);
	if (ret.C_API != COMPACT_API)
	{
		utils << "Warring:CompactAPI is mismatch!" << endl;
		utils << ret.C_API << " is not equal of " << COMPACT_API << endl;
		stat = false;
	}
	utils<<"dePackage CheckSUM:"<<xbit((char*)&ret, HEAD_LEN)<<endl;
	return ret;
}

void Package(const CONN_HEAD head, char *buff)
{
	display_dump(buff, HEAD_LEN);
	memcpy(buff, &head, HEAD_LEN);
	utils << "Package CheckSUM:" << xbit((char*)&head, HEAD_LEN) << endl;
	//crypt(buff, HEAD_LEN, DEFAULT_PASSWORD);
}