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
	inline void operator<<(string str)
	{
		std::cout << SearchStr(str);
	}
	inline translate() {};
};


#define cout translate