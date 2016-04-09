#pragma once
#include "main.h"
#include "libSemphella.h"
#include "debug.h"

enum STAT {
	OK,
	FAILD,
	VerifyPackageFaild,
	Fixed,
	MultiMatch,
	NoMatch,
};

API void CaculateRecovery(const char *data, int64_t len, char *ret);
API STAT Recovery(char *data, int64_t len, const char *rec);
API STAT VerifyRecoveryData(const char *rec, int64_t len);
API int64_t get_block_len(int64_t data_len);