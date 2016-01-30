#pragma once
#include "kernel.h"
inline void display_dump(const char *data, long long len, int x = 5)
{
	for (int n = 0; n < len; n++)
	{
		char buff[MAX_BUFF_SIZE] = { 0x00 };
		sprintf(buff, "%02X", data[n]);
		cout << buff[strlen(buff) - 2] << buff[strlen(buff) - 1] << "  ";
		if (n%x == 0)
			cout << endl; 
	}
	cout << endl;

}

#define debug if (KERNEL.GetDebugStat())cout<<"["<<__FILE__<<"]["<<__LINE__<<"]"
