#include "libSkovia.h"


class AINIT {
public:
	AINIT();
};

AINIT::AINIT()
{
	debug << "Init Windows WSA" << endl;
	WSAData wsa;
	WORD word = MAKEWORD(2, 2);
	WSAStartup(word, &wsa);
	debug << "WSA Is Inited" << endl;
}

AINIT ainit;