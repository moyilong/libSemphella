#pragma once
#include "kernel.h"
#include "config.h"

inline string __debug_strgetfilename(string filename)
{
	int last = 0;
	for (int n = filename.size()-1; n >0; n--)
		if (filename.at(n) == '\\' || filename.at(n) == '/')
		{
			last = n;
			break;
		}
	return filename.substr(last+1);
}

#ifdef __ALLOW_DYMANIC_DEBUG
#define DEBUG_LINE	if (KERNEL.GetDebugStat())
#else
#define DEBUG_LINE if(__DEFAULT_DEBUG_STAT)
#endif

#define debug DEBUG_LINE cout<<"["<<__debug_strgetfilename(__FILE__)<<"]["<<__LINE__<<"]"

#define DEBUG_CALL_TRACK_ARGMENT	string __file_name=__FILE__ ,int __file_line=__LINE__
#define DEBUG_CALL_TRACK_ARGMENT_FUNC	string __file_name,int __file_line

API void __display_dump(const char *data, long long len, int x = 5, DEBUG_CALL_TRACK_ARGMENT);



#define display_dump DEBUG_LINE __display_dump