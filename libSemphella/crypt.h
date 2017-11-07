#pragma once
#include "libSemphella.h"
#include <limits>
#include "emmx.h"
API char get_off();

#ifndef _EL_CPYOFF
#define _EL_CPYOFF  0x5F
#endif

#define count_t unsigned  long long

API char xbit(const char *data, long long len, const char off = _EL_CPYOFF);
CAPI void crypt(char* data, long long len, const char *password);

CAPI void word_list_convert(char *str, const char *word_white_list = DEFAULT_WORD_WHITE_LIST
	, const char *word_balck_list = DEFAULT_WORD_BLACK_LIST);

API bool word_list_check(const char *word_white_list, const  char *word_black_list);
CAPI void crypt_t(char *data, long long len, const char *password, const char *la = DEFAULT_WORD_WHITE_LIST, const char *lb = DEFAULT_WORD_BLACK_LIST);

API void dymanic_code(const char *license, long long license_len, string password, string &ret);
API void half_dymanic_code(const char *license, long long license_len, string password, string &ret, count_t now);

//inline feature

inline void crypt_t(char *data, long long len, string password, const char *la = DEFAULT_WORD_WHITE_LIST, const char *lb = DEFAULT_WORD_BLACK_LIST)
{
	crypt_t(data, len, password.data(), la, lb);
}
inline void word_list_deconvert(char *str, const char *word_white_list = DEFAULT_WORD_WHITE_LIST
	, const char *word_balck_list = DEFAULT_WORD_BLACK_LIST)
{
	word_list_convert(str, word_balck_list, word_white_list);
};

inline void crypt(char *data, long long len, const string password)
{
	crypt(data, len, password.data());
}

API char xor_crypt(string password, char *data, int len);
API float getsum(const char *data, int len);

API uint64_t getsumV2(const char *data, int64_t len);

#define anySUM(ptr) getsumV2((char*)ptr,sizeof(ptr));

API void fastCrypt(char *data, int64_t len, string password, int PMLEN = 16);
inline void fastCrypt_128(char *data, int64_t len, string password)
{
	fastCrypt(data, len, password, 16);
}
inline void fastCrypt_256(char *data, int64_t len, string password)
{
	fastCrypt(data, len, password, 32);
}

inline void fastCrypt_4096(char *data, int64_t len, string password)
{
	fastCrypt(data, len, password, 512);
}

API void fcTest();
API void mask(char *buff, int64_t len);
API void pre_calc_pct(int64_t len);
API void aesTest();
API void randomMatch(char *data, uint64_t len, const char *meed=NULL);
typedef void(*crt_algr_func)(char *data, int64_t len, string password);
API bool algrTest(crt_algr_func algr, int test_len = 128);

#undef min
#undef max
API vector<string> getsum2_decrypt(uint64_t sum, uint64_t begin_seek = numeric_limits<uint64_t>::min(), uint64_t end_seek = numeric_limits<uint64_t>::max(), string allow_string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/*-+.`~!@#$%^&*()_+-=\\|;':\"[]{},.<>/?");
#define DGST_LEN 8192
struct dgst {
	char data[DGST_LEN];
	uint64_t main_data_verify;
	inline dgst()
	{
		memset(data, 0, sizeof(data));
		main_data_verify = 0;
	}
};

API bool dgst_check(const dgst dgst);
API dgst dgst_calc(const char *data, int64_t len);
API dgst dgst_merge(const dgst a, const dgst b);
API string dgst_string(const dgst a);
API emmx mpSum(const char *data, uint64_t len, int caluc_length);
API void mpSum_Test(int test_length, bool v2_algr = false);
API uint64_t mpsum2(const char *data, uint64_t len);

API string Base64Encode(const string& src);

typedef uint64_t(*hash_api)(const char *data, const uint64_t len);