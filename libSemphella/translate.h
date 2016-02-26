#pragma once
#include "libSemphella.h"
#include "main.h"

struct TRANSLATE_BLOCK {
	string orig;
	string trans;
};


API vector<TRANSLATE_BLOCK> tpoll;

API bool LoadFromFile(string filename);
API void Register(TRANSLATE_BLOCK tblock);
inline void Register(TRANSLATE_BLOCK *tblock, int size)
{
	for (int n = 0; n < size; n++)
		Register(tblock[n]);
}
API string SearchStr(string orig);

class translate {
public:
	inline void operator<<(string str)
	{
		cout << SearchStr(str);
	}
};