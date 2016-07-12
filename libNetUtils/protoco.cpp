#include "protoco.h"

WORK_CMD PortGet(SOCKET sock, void * ptr, uint64_t &len)
{
	HEAD_PROTOCO protoco;
	recv(sock, (char*)&protoco, sizeof(HEAD_PROTOCO), 0);
	if (protoco.length == 0)
	{
		ptr = NULL;
		len = 0;
		return protoco.cmd;
	}
	len = protoco.length;
	ptr = malloc(protoco.length);
	recv(sock, (char*)ptr, len, 0);
	if (protoco.md_verify != getsumV2((char*)ptr, len))
		throw MD_VERIFY_ERROR;
	return protoco.cmd;
}

void PortSend(SOCKET sock, void * ptr, uint64_t len, WORK_CMD cmd)
{
	HEAD_PROTOCO head;
	head.cmd = cmd;
	head.length = len;
	if (len != 0)
		head.ver = getsumV2((char*)ptr, len);
	send(sock, (char*)&head, sizeof(HEAD_PROTOCO), 0);
	if (len != 0)
		send(sock, (char*)ptr, len, 0);
}
