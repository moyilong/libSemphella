#pragma once
#include "libSemphella.h"
#include <inttypes.h>
#include "apd.h"
#include "VER.h"
enum KTYPE {
	LINUX,
	WINDOWS,
};

enum REG_TYPE {
	INIT,
	INEXIT,
};

typedef void(*KSAPI)();

class CAPI kernel
{
public:
	kernel();
	~kernel();
	time_t get_start_time();
	void abort();
	void _message(string info, string file, int line);
	void _error(string info, string file, int line);
	bool GetDebugStat();
	void _SetDebugStat(bool stat, string file, int line);
	void LogoPrint();
	void Register(REG_TYPE reg, KSAPI api);
	VER GetVer();
	int get_api_ver();
#define message(info) _message(info,__FILE__,__LINE__)
#define error(info) _error(info,__FILE__,__LINE__)
#define SetDebugStat(info) _SetDebugStat(info,__FILE__,__LINE__)
private:
	time_t start_time;
	vector<KSAPI> init_call;
	vector<KSAPI> exit_call;
};

extern "C" API kernel KERNEL;

#define EVENT_CALL (type,func) KERNEL.Register(type,func)