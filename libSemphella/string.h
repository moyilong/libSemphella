#pragma once
#include "main.h"
#include "debug.h"
API int strfind(const char *str,char find,bool wn=false);
CAPI void strcpy(char *dest,const char *origin,long long cplen,long long r_off,long long w_off);
API string & operator<<(string input, string x);
API bool streval(const char *a, const char *b,bool over_len=false);
API char* eitoa(int num, char*str, int radix,const char *word_list=DEFAULT_WORD_WHITE_LIST);
API bool is_upper(const char bit);
API char set_upper(const char bit, bool upper=true);
API string upper_string(string str, bool upper=true);
API bool is_str(const char bit);
API string ull2s(uint64_t value);
inline string int2s(int value) { return ull2s(value); }
template<class T> inline T get_procs(T type) { return type; }
API string strreplace(string orig, string replace, string value);
API string strrm(const char* str, const char *rm_list);

API string human_read(uint64_t _in, string *unit, int step,int number_out_type);
API string human_read_storage_str[];

inline string eitoa(int num, int radix,const char *word_list=DEFAULT_WORD_WHITE_LIST)
{
	char buff[MAX_BUFF_SIZE];
	memset(buff, 0, sizeof(buff));
	eitoa(num, buff,radix, word_list);
	return buff;
}