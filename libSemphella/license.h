#pragma once
#include "main.h"
#include "math.h"
#include "libSemphella.h"
#include "debug.h"
#include "crypt.h"
#include "string.h"

struct CAPI LICENSE {
	string main;
	string check;
	LICENSE LICENSE::operator =(string str);
	string LICENSE::operator =(LICENSE lic);
};

API bool CheckLicense(LICENSE lic);
API LICENSE CreateLicense(uint64_t _seed, uint64_t leng_bit);

API char ArgmentGetValue(LICENSE lic, uint64_t arg1, uint64_t arg2);
