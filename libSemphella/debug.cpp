#include "debug.h"
API void __display_dump(const char * data, long long len, int x, DEBUG_CALL_TRACK_ARGMENT_FUNC)
{
	if (!KERNEL.GetDebugStat())
		return;
#define TRACK_COUT	 DEBUG_LINE cout<<"["<<__file_name<<"]["<<__file_line<<"][HEX DUMP]"
	TRACK_COUT << endl;
	TRACK_COUT;
	for (int n = 0; n < len; n++)
	{
		char buff[MAX_BUFF_SIZE] = { 0x00 };
		sprintf(buff, "%02X", data[n]);
		cout << buff[strlen(buff) - 2] << buff[strlen(buff) - 1] << "  ";
		if ((n + 1) % x == 0 && n != 0)
		{
			cout << endl;
			TRACK_COUT;
		}
	}
	cout << endl;
}

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