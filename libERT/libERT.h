#pragma once
#include "dll.h"
#include "stdafx.h"
#include "headproto.h"

enum RETURN_STAT {
	OK,
	FILE_IO_FAILD,
	ALG_FAILD,
	FID_FAILD,
	EXT_NOT_EXIST,
	HEAD_VERIFY_FAILD,
	FILE_VERIFY_FAILD,
	PASSWORD_FAILD,
	EXT_VERFY_FAILD,
};
#define DEFAULT_ALG_ID	10
#define DEFAULT_FHL_ID	0

typedef void(*ProgressAPI)(double percent);

LIBERT_API RETURN_STAT crypt_to_file(string in, string out, string password, int alg = DEFAULT_ALG_ID, int fid = DEFAULT_FHL_ID, string extfil = "", int bs = 1 * 1024 * 1024,ProgressAPI api=NULL);
LIBERT_API RETURN_STAT decrtpt_to_file(string in, string out, string password, int std_mode = false, ProgressAPI api=NULL);
LIBERT_API RETURN_STAT decrypt_to_std(string in, string out, string password,ProgressAPI api =NULL);
LIBERT_API RETURN_STAT get_ext_to_file(string in, string out, bool std_mode = false);
LIBERT_API RETURN_STAT get_ext_to_std(string in);
LIBERT_API HEAD get_head(string in);
LIBERT_API int PerformanceTest(int timeout=5);

LIBERT_API int algor_max();
LIBERT_API int fhand_max();

LIBERT_API void get_alg_info(int stor_id, int &id, string &help);
LIBERT_API int get_alg_id(int tid);

LIBERT_API string get_api_ver();