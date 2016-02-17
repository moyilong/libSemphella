#pragma once
#include "kernel.h"
#include "config.h"

#ifdef __ALLOW_DYMANIC_DEBUG
#define DEBUG_LINE	if (KERNEL.GetDebugStat())
#else
#define DEBUG_LINE if(__DEFAULT_DEBUG_STAT)
#endif

#define debug DEBUG_LINE cout<<"["<<__FILE__<<"]["<<__LINE__<<"]"

#define DEBUG_CALL_TRACK_ARGMENT	string __file_name=__FILE__ ,int __file_line=__LINE__


inline void __display_dump(const char *data, long long len, int x = 5,DEBUG_CALL_TRACK_ARGMENT)
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
		if ((n+1)%x == 0 && n != 0)
		{
			cout << endl;
			TRACK_COUT;
		}
	}
	cout << endl;
}


#define display_dump DEBUG_LINE __display_dump