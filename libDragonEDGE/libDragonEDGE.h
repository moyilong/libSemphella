#pragma once

/*

ELONE inside - DragonEDGE Project

*/

/*
工作流程:

		宿主机调用	->	接受数据	->	返回数据	->	结束

*/

#include <libSemphella/libSemphella.h>
#include <libSemphella/debug.h>
#include <libSemphella/crypt.h>

#define MAX_BUFF_SIZE	8192
#define API_VERSION	0x5D

#define GUID	long long


#define PACKAGE_FAILD	0xFFFF0000
#define PACKAGE_UNDEF	0xFFFF0001


struct IO_BUFF {
	char VER;
	long long cmdcall;
	GUID id;
	char swap[MAX_BUFF_SIZE];
	char sum;
	inline IO_BUFF()
	{
		cmdcall = 0;
		sZero(swap, 0, MAX_BUFF_SIZE);
	}
};

#define IO_LEN	sizeof(IO_BUFF)

typedef void(*driver)(char *buff, long long &len);
typedef void(*void_type)(void);

struct driver_t
{
	driver send = NULL;
	driver recv = NULL;
	void_type init = NULL;
	bool inited = false;
};

void service(driver_t &drv);
void client(driver_t &drv);

typedef void(*mod_entry)(IO_BUFF &io, const IO_BUFF get);

class modules {
public:
	modules(long long xcall, mod_entry api);
	modules(long long xcall, mod_entry api, void_type xloop);
	long long call;
	mod_entry ent;
	void_type loop;
};