#pragma once
#include "libSemphella.h"
#include <inttypes.h>

enum KTYPE {
	LINUX,
	WINDOWS,
};

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
	void SetDebugStat(bool stat,string file=__FILE__,int line = __LINE__);
	void LogoPrint();
private:
	time_t start_time;
};

extern "C" API kernel KERNEL;
