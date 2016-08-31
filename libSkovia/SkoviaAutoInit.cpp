#include "libSkovia.h"


class AINIT {
public:
	AINIT();
};

AINIT::AINIT()
{
#ifndef __LINUX__
	debug << "Init Windows WSA" << endl;
	WSAData wsa;
	WORD word = MAKEWORD(2, 2);
	WSAStartup(word, &wsa);
	debug << "WSA Is Inited" << endl;
#endif
}

AINIT ainit;