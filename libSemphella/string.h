#pragma once
#include "libSemphella.h"
#include "debug.h"



API string StrLimit(string str, int len = 8, bool spare = false);
API int strfind(const char *str, char find, bool wn = false);
CAPI void strcpy(char *dest, const char *origin, long long cplen, long long r_off, long long w_off);
API bool streval(const char *a, const char *b, bool over_len = false);
API char* eitoa(int num, char*str, int radix, const char *word_list = DEFAULT_WORD_WHITE_LIST);
API bool is_str(const char bit);
API string ull2s(uint64_t value);
inline string int2s(int value) { return ull2s(value); }
template<class T> inline T get_procs(T type) { return type; }
API string strreplace(string orig, string replace, string value);
API string convert_process(const char *data);
API string human_read(uint64_t _in, string *unit, int step);
API string api_human_read_time_unit(uint64_t val);
API string api_human_read_storage_unit(uint64_t val);
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
API double atolf(string val);
API bool StrMatch(const char *str, const char *match);

API bool operator == (const char * stra, const string strb);
API bool operator == (const string stra, const char *strb);

#define strequal streval

inline bool is_upper(const char bit)
{
	if (!is_str(bit))
		return false;
	if ('A' >= bit && 'Z' <= bit)
		return true;
	return false;
}

inline char set_upper(const char bit, bool upper)
{
	if (!is_str(bit))
		return false;
	if (is_upper(bit))
		return bit + ('a' - 'A');
	else
		return bit - ('a' - 'A');
}

class API estring : public string
{
public:
	vector<estring> Split(char arr = '\n', bool auto_trim = false);
	inline estring(const char *str) : string(str) {};
	inline estring(string str) : string(str) {};
	inline estring() : string("") {};
	inline char operator[](int id)
	{
		return data()[id];
	}
	inline bool operator==(const char *str)
	{
		return strequal(data(), str);
	}
	estring ToUpper();
	estring ToLower();
	estring Trim();
	estring RemoveCharArray(const char *arr);
	inline bool empty()
	{
		return (length() == 0);
	}
	static inline estring format(const char *format, void *data)
	{
		char buff[MAX_BUFF_SIZE];
		sprintf(buff, format, data);
		return estring(buff);
	}
	static inline estring connect(vector<estring>str, estring connector)
	{
		estring ret;
		for (int n = 0; n < str.size(); n++)
			ret += connector + str[n];
		return ret.substr(connector.size()+1);
	}
};
