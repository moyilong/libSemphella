#pragma once
#include "libSemphella.h"
#include "debug.h"
#include "estring.h"

API string StrLimit(string str, int len = 8, bool spare = false);
API int strfind(const char *str, char find, bool wn = false);
CAPI void strcpy(char *dest, const char *origin, long long cplen, long long r_off, long long w_off);
API bool streval(const char *a, const char *b, bool over_len = false);
API char* eitoa(int num, char*str, int radix, const char *word_list = DEFAULT_WORD_WHITE_LIST);
API bool is_upper(const char bit);
API char set_upper(const char bit, bool upper = true);
API string upper_string(string str, bool upper = true);
API bool is_str(const char bit);
API string ull2s(uint64_t value);
inline string int2s(int value) { return ull2s(value); }
template<class T> inline T get_procs(T type) { return type; }
API string strreplace(string orig, string replace, string value);
API string strrm(const char* str, const char *rm_list);
API string convert_process(const char *data);
API string human_read(uint64_t _in, string *unit, int step);
API string api_human_read_time_unit(uint64_t val);
API string api_human_read_storage_unit(uint64_t val);
API vector<estring> Splite(string val, const char spliter, const bool auto_trim = false);
API string Trim(string val);
#define human_read_time_unit api_human_read_time_unit
#define human_read_storage_str api_human_read_storage_unit

inline string eitoa(int num, int radix, const char *word_list = DEFAULT_WORD_WHITE_LIST)
{
	char buff[MAX_BUFF_SIZE];
	memset(buff, 0, sizeof(buff));
	eitoa(num, buff, radix, word_list);
	return buff;
}

API string AddressGetFileName(string filename);
API string f2s(float value);
API bool memequal(const void * a, const void * b, uint64_t size);
API vector<string> splite(string val, char spliter);
API double atolf(string val);
API bool StrMatch(const char *str, const char *match);

API bool operator == (const char * stra, const string strb);
API bool operator == (const string stra, const char *strb);

#define strequal streval