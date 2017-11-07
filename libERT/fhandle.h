#pragma once
#include <libSemphella/libSemphella.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>
#include "libERT.h"
#include "external_library.h"
#define FHANDLE_MAX	16

class FHANDLE {
public:
	FHANDLE(CFHANDLE hand, int lid);
	FHANDLE(fhand_t ht);
	inline FHANDLE() {};
	int id;
	CFHANDLE hand;
};

extern FHANDLE FPOLL[FHANDLE_MAX];

extern int fsize;
CFHANDLE get_fhandle(int n);
#define XFHANDLE static FHANDLE