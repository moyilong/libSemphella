#pragma once
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>
#include "libERT.h"
#define FHANDLE_MAX	16
typedef void(*CFHANDLE)(HEAD head, file _in, file _out, uint64_t &sum, int len, uint64_t op_addr, bool decrypt, bool std_out);
class FHANDLE {
public:
	FHANDLE(CFHANDLE hand, int lid);
	inline FHANDLE() {};
	int id;
	CFHANDLE hand;
};

extern FHANDLE FPOLL[FHANDLE_MAX];

extern int fsize;
CFHANDLE get_fhandle(int n);
#define XFHANDLE static FHANDLE