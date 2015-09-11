#include "stdafx.h"
#include "modules.h"
#include "protoco.h"
int protoco = 0;

void Protoco_Scan_Server(DATA_FORMAT in, DATA_FORMAT &ret, SOCKET &conn)
{
	PROTOCO_INFO getd;
	PROTOCO_INFO ret_p;
	memcpy(&getd, in.buff, sizeof(PROTOCO_INFO));
	if (getd.protoco_count != protoco)
		ret_p.protoco_count = protoco;
	memcpy(ret.buff, &ret_p, sizeof(PROTOCO_INFO));
}

void Protoco_Scan_Client_to(DATA_FORMAT &in)
{
	PROTOCO_INFO temp;
	temp.protoco_count = protoco;
	memcpy(in.buff, &temp, sizeof(PROTOCO_INFO));
}

void Protoco_Scan_Client_ret(DATA_FORMAT &ret)
{
	PROTOCO_INFO temp;
	memcpy(&temp, ret.buff, sizeof(PROTOCO_INFO));
	if (temp.protoco_count != protoco)
	{
		char buff_script[MAX_BUFF_SIZE] = { 0x00 };
		sprintf(buff_script, "%d", protoco);
		string C_HEAD = COMMAND_HEAD;
		string S_DIR = SCRIPT_DIR;
		string exec = C_HEAD + S_DIR + "//" + buff_script + ".script &";
		system(exec.data());
		protoco = temp.protoco_count;
	}
}

Modules Protoco_Scan(PROTOCO_API, "ProtocoScanner",Protoco_Scan_Server, Protoco_Scan_Client_to, Protoco_Scan_Client_ret);