#pragma once
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>
#undef max
#undef min
#include <limits>
#include "algorthim.h"

struct MP_BLOCK {
	int ilen;
	double iops;
	uint64_t count = 0;
	string temp;
	uint64_t n = 0;
	double per;
};

#define FILE_TYPE ".ert3"
//static const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,/.<>?;':\"[] {}|\\-+*/";
#define strtbl "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

//static const int level = 2;
//static const int level_compact = 2;
#define level 5
#define level_compact 2
#define ext_support_lab	(char)'S'
#define ext_end_lab	(char)'E'
#define DEFAULT_FHANDLE 0
#define DEFAULT_ALGRTHOM_TYPE	6

#define cp2 debug
#define DEBUG debug
#define MAX_PASSWORD_LEN	MAX_BUFF_SIZE
#define APOLL_SIZE	xsize
#define APOLL_IDMAX	xsize

#define EXT_SIZE	sizeof(uint64_t)

//#define WHITE_CRYPT

struct HEAD {
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
	void reset_ext(bool faild_test=false);
};

#define DESIGN_KEEP_SIZE	4096

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
extern char*ext_data;
void logo();
bool file_name_check(string filename);
extern EXT ex;
extern int64_t bs;
extern bool crack;
extern bool std_out;
extern bool force;
extern bool crack_get;
extern bool info_get;;
extern int alghtriom;;
extern int fhand;
extern int al;
extern string input;
extern string output;
extern string password;

int crypt_process();
int information_process();
int create_license();
void PerformanceTest();
void ext_dump();