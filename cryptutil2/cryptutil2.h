#pragma once
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>
#undef max
#undef min
#include <limits>
#include <libERT/libERT.h>

#define FILE_TYPE ".ert3"
//static const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,/.<>?;':\"[] {}|\\-+*/";
#define strtbl "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

#define cp2 debug
#define DEBUG debug
#define MAX_PASSWORD_LEN	MAX_BUFF_SIZE

enum WORK_MODE {
	FILE_INFO,
	CRYPT,
	DECRYPT,
	LICENSE_CREATE,
	EXT_TO_FILE,
};

int create_license(string output, bool std_out, int bs);