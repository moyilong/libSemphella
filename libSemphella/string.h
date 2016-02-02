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
API string human_read(uint64_t in);
API string ull2s(uint64_t value);
inline string int2s(int value) { return ull2s(value); }
template<class T> inline T get_procs(T type) { return type; }

inline string strreplace(string orig, string replace, string value)
{
	display_dump(orig.data(), orig.size());
	display_dump(replace.data(), replace.size());
	string ret;

	for (int n = 0; n < orig.size(); n++)
	{
		bool check = true;
		if (orig.at(n) == replace.at(0))
		{
			for (int x = 0; x < replace.size(); x++)
				if (n + x  < orig.size()+1)
				{
					if (orig.at(n + x) != replace.at(x))
					{
						check = false;
					}
				}
				else {
					check = false;
				}

		}
		else {
			check = false;
		}
		if (check)
		{
			ret += value;
			n += replace.size() + 1;
		}
		else {
			ret += orig.at(n);
		}
	}
	return ret;
}

API string strrm(const char* str, const char *rm_list);