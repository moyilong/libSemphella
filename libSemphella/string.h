#pragma once
#include "main.h"
API int strfind(const char *str,char find,bool wn=false);
CAPI void strcpy(char *dest,const char *origin,long long cplen,long long r_off,long long w_off);
API string strreplace(const char*origin, const char *replace, const char*value);
API string & operator<<(string input, string x);
API bool streval(const char *a, const char *b,bool over_len=false);
API char* eitoa(int num, char*str, int radix,const char *word_list=DEFAULT_WORD_WHITE_LIST);
API bool is_upper(const char bit);
API char set_upper(const char bit, bool upper=true);
API string upper_string(string str, bool upper=true);
API bool is_str(const char bit);
API string human_read(uint64_t in);

template<class T> inline T get_procs(T type) { return type; }