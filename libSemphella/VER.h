#pragma once
#include "main.h"
struct VER {
	int main;
	int build;
	int version;
	int fix;
	inline string to_str()
	{
		char buff[128];
		sprintf(buff, "%d.%d.%d.%d", main, build, version, fix);
		return buff;
	}
	inline VER(int _m, int _b, int _v, int _f)
	{
		main = _m;
		build = _b;
		version = _v;
		fix = _f;
	}
	inline VER() {};
};