#pragma once
#include "kernel.h"
#include "config.h"
#include "string.h"
#include "utils.h"
#ifdef __ALLOW_DYMANIC_DEBUG
#define DEBUG_LINE	if (KERNEL.GetDebugStat())
#else
#define DEBUG_LINE if(__DEFAULT_DEBUG_STAT)
#endif

#define debug DEBUG_LINE cout<<"["<<AddressGetFileName(__FILE__)<<"]["<<__LINE__<<"]["<<__FUNCTION__<<"]"

#define display_dump DEBUG_LINE HexDump

enum hdebug_preset {
	DEBUG_UNSET = 0xffffffff,
	DEBUG_OK = 0xc0000000,
};

class API hdebug {
public:
	hdebug(uint64_t val);
	hdebug();
	uint64_t GetVal();
	string Val2Str();

private:
	uint64_t ival;
};