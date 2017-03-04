#include "debug.h"
hdebug::hdebug(uint64_t val)
{
	ival = val;
}

hdebug::hdebug()
{
	ival = DEBUG_UNSET;
}

uint64_t hdebug::GetVal()
{
	return ival;
}

string hdebug::Val2Str()
{
	return ull2s(ival);
}

API const char *error_list[] = {
	"INVALID_ARGMENT",
	"INVALID_DICT_FIND",
};