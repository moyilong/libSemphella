#pragma once
#include "include.h"
#include <libSemphella/sysapi_modcall.h>

typedef void(*CAT_API)(char *val, int len,argment args);
typedef void(*CAT_PROC)(argment args);

struct _CAT_MOD {
	_CAT_MOD(string name, CAT_API api,  CAT_PROC init_call = NULL, CAT_PROC endcall = NULL);
	inline _CAT_MOD() {};
	CAT_API dapi;
	CAT_PROC dinit_call;
	CAT_PROC dendcall;
	string dname;
};

#define CAT_MOD static _CAT_MOD


_CAT_MOD Search(string name);