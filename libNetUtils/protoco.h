#pragma once
#include "libNetUtils.h"
#define HEAD_VER	0x55

struct HEAD_PROTOCO {
	char ver;
	WORK_CMD cmd;
	uint64_t md_verify;
	uint64_t length;
};

WORK_CMD PortGet(SOCKET sock, void *ptr, uint64_t &len);
void PortSend(SOCKET sock, void *ptr, uint64_t len, WORK_CMD cmd);
