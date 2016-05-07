#pragma once
#include "stdafx.h"
#define level 5
#define level_compact 2
#define DESIGN_KEEP_SIZE	4096
#define EXT_SIZE	sizeof(uint64_t)
#include "dll.h"
struct LIBERT_API HEAD {
	char account_level = level;
	char algrthom;
	uint64_t sum;
	//uint64_t matrix_sum;
	char ext[EXT_SIZE];
	uint64_t password_sum;
	uint64_t bs = bs;

	//Program Funcation
	bool check();
	HEAD();
	void reset_ext(bool faild_test = false);
};

struct EXT {
	const char start_bit = 0xB0;
	uint64_t length;
	uint64_t checksum;
	const char stop_bit = 0xB1;
	char buff[DESIGN_KEEP_SIZE - sizeof(uint64_t) * 2];
	const char stop_all = 0xBA;
};
#define EXT_SUPPORT	0
#define EXT_FHANDLE	1
#define EXT_EXTABLE	2
#define EXT_ENDFLAG	EXT_SIZE-1

#define APOLL_SIZE	xsize
#define APOLL_IDMAX	xsize

#define ext_support_lab	(char)'S'
#define ext_end_lab	(char)'E'

#define DEFAULT_FHANDLE 0
#define DEFAULT_ALGRTHOM_TYPE	6

extern EXT ex;