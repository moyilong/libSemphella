#pragma once

API int strfind(const char *str,char find);
API void strcpy(char *dest,const char *origin,long long cplen,long long r_off,long long w_off);
API string strreplace(const char*origin, const char *replace, const char*value);
API string & operator<<(string input, string x);
API bool streval(const char *a, const char *b);
API char* eitoa(int num, char*str, int radix);


