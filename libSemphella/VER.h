#pragma once
#include "main.h"

#ifndef __LINUX__
#pragma warning(disable:4996)
#endif

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
		//return ull2s(main) + "." + ull2s(build) + "." + ull2s(version) + "." + ull2s(fix);
	}

	inline VER(const int _m, const int _b,const  int _v,const int _f)
	{
		main = _m;
		build = _b;
		version = _v;
		fix = _f;
	}

	inline VER() {
		main = 0;
		build = 0;
		version = 0;
		fix = 0;
	};

	inline VER operator=(const VER ver)
	{
		main = ver.main;
		build = ver.build;
		version = ver.version;
		fix = ver.version;
		return *this;
	}

	inline bool operator==(const VER ver)
	{
		if (main != ver.main || build != ver.build || version != ver.version || fix != ver.fix)
			return false;
		return true;
	}
};

