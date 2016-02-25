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

#define cp2 debug<<"[crypt2]"
#define FILE_TYPE ".ert2"
//static const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,/.<>?;':\"[] {}|\\-+*/";
#define strtbl "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

//static const int level = 2;
//static const int level_compact = 2;
#define level 2
#define level_compact 2

#ifdef LOW_PERFORMANCE_DEVICE
#define DEFAULT_ALGRTHOM_TYPE	2
#else
#define DEFAULT_ALGRTHOM_TYPE	3
#endif

#define DEBUG debug
#define MAX_PASSWORD_LEN	MAX_BUFF_SIZE

#define APOLL_SIZE	(APOLL.size())
#define APOLL_IDMAX	(APOLL_SIZE-1)

struct HEAD {
	char account_level = level;
	char algrthom;
	uint64_t sum;
	uint64_t matrix_sum;
	uint64_t password_sum;
	uint64_t bs = bs;
	bool check();
	HEAD();
};

void logo();
bool file_name_check(string filename);

extern int64_t bs;
extern bool crack;
extern bool std_out;
extern bool force;
extern bool crack_get;
extern bool info_get;;
extern int alghtriom;;
extern int al;
extern string input;
extern string output;
extern string password;

int crypt_process();
int information_process();