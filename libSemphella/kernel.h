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
	void message(string info, string file = __FILE__, int line = __LINE__);
	void error(string info, string file = __FILE__, int line = __LINE__);
	bool GetDebugStat();
	void SetDebugStat(bool stat, string file = __FILE__, int line = __LINE__);
	void LogoPrint();
	void Register(REG_TYPE reg, KSAPI api);
	VER GetVer();
	int get_api_ver();
private:
	time_t start_time;
	vector<KSAPI> init_call;
	vector<KSAPI> exit_call;
	VER kver;
};

extern "C" API kernel KERNEL;

#define EVENT_CALL (type,func) KERNEL.Register(type,func)