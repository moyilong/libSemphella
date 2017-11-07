#pragma once
#include "libSemphella.h"
#include "string.h"
#include <string>
using namespace std;
class estring : string
{
public:
	vector<estring> Split(char arr = '\n',bool auto_trim=false);
	estring operator= (string s)
	{
		return s.data();
	}
	
};

