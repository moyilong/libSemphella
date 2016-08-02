#pragma once
#include "libSemphella.h"
#include "main.h"

struct TRANSLATE_BLOCK {
	string orig;
	string trans;
};

API bool LoadFromFile(string filename);
API void Register(TRANSLATE_BLOCK tblock);
API string SearchStr(string orig);

inline void Register(TRANSLATE_BLOCK *tblock, int size)
{
	for (int n = 0; n < size; n++)
		Register(tblock[n]);
}

class CAPI translate {
public:
	inline void operator<<(const string str)
	{
		std::cout << SearchStr(str);
	}
	inline void operator<<(const char *str)
	{
		std::cout << SearchStr(str);
	}
	inline void operator<<(const uint64_t val)
	{
		std::cout << val;
	}
	inline void operator<<(const int64_t val)
	{
		std::cout << val;
	}
	inline translate() {};
};

extern translate template_x;
#define endl "\n"