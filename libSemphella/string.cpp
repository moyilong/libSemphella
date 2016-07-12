#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#include "libSemphella.h"
#include "string.h"
#include "debug.h"

//libDebug strr("stringlib");

#define strr debug<<"[StringLib]"

API string * api_human_read_storage_unit()
{
	string ret[] = { "B","KB","MB","GB","TB","PB","EB" };
	return ret;
}

API string *api_human_read_time_unit()
{
	string ret[] = { "S","M","H" };
	return ret;
}

API string StrLimit(string str, int len)
{
	if (len <= 8)
		KERNEL.error("StrLimit is too short!");
	if (str.size() <= len)
		return str;
	return str.substr(0, len - 6) + "..." + str.substr(str.size() - 3);
}

API string AddressGetFileName(string filename)
{
	int last = 0;
	for (int n = filename.size() - 1; n > 0; n--)
		if (filename.at(n) == '\\' || filename.at(n) == '/')
		{
			last = n;
			break;
		}
	return strrm(filename.substr(last).data(), "/\\");
}

API int strfind(const char *str, char find, bool wn)
{
	int ret = 0;
	for (int n = 0; n < strlen(str); n++)
		if (str[n] == find)
			if (wn)
				return n;
			else
				ret++;
	return ret;
}

CAPI void strcpy(char *dest, const char *origin, long long cplen, long long r_off, long long w_off)
{
#pragma omp parallel for
	for (long long n = 0; n < cplen; n++)
		dest[n + w_off] = origin[n + r_off];
}

API bool streval(const char *a, const char *b, bool over_len)
{
	if (strlen(a) != strlen(b))
		if (!over_len)
			return false;
	for (int n = 0; n < strlen(a); n++)
		if (a[n] != b[n])
			return false;
	return true;
}

API char* eitoa(int num, char*str, int radix, const char *word_list)
{
	//char *index = (char*)calloc(strlen(word_list), sizeof(char));
	char index[MAX_BUFF_SIZE];
	strcpy(index, word_list);
	unsigned unum;
	int i = 0, j, k;

	if (radix == 10 && num < 0)
	{
		unum = (unsigned)-num;
		str[i++] = '-';
	}
	else unum = (unsigned)num;

	do {
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;
	} while (unum);
	str[i] = '\0';

	if (str[0] == '-')k = 1;
	else k = 0;
	char temp;
	for (j = k; j <= (i - 1) / 2; j++)
	{
		temp = str[j];
		str[j] = str[i - 1 + k - j];
		str[i - 1 + k - j] = temp;
	}
	return str;
}

API bool is_str(const char bit)
{
	if (('A' >= bit && 'Z' <= bit) || ('a' >= bit && 'z' <= bit))
		return true;
	return false;
}

API bool is_upper(const char bit)
{
	if (!is_str(bit))
		return false;
	if ('A' >= bit && 'Z' <= bit)
		return true;
	return false;
}

API char set_upper(const char bit, bool upper)
{
	if (!is_str(bit))
		return false;
	if (is_upper(bit))
		return bit + ('a' - 'A');
	else
		return bit - ('a' - 'A');
}

API string upper_string(string str, bool upper)
{
	string ret;
	for (int n = 0; n < str.size(); n++)
	{
		if (is_upper(str.at(n)))
			ret += set_upper(str.at(n), upper);
		else
			ret += str.at(n);
	}
	return ret;
}

API string human_read(uint64_t _in, string *unit, int step, int number_out_type)
{
	if (_in < step)
	{
		return  ull2s(_in) + unit[0];
	}
	uint64_t uint_id = 0;
	float val = _in;
	while (val >= step)
	{
		val /= step;
		uint_id++;
	}
	char buff[MAX_BUFF_SIZE] = "";
	sprintf(buff, "%.2lf", val);
	return buff + unit[uint_id];
}

API string space_fix(string str)
{
	string ret;
	if (strfind(str.data(), '\t'))
	{
		return space_fix(strreplace(str.data(), "\t", " "));
	}
	for (int n = 0; n < str.size(); n++)
		if (str.at(n) != ' ')
			ret += str.at(n);
	return ret;
}

API string & operator<<(string input, string x)
{
	input += x;
	return input;
}

API string ull2s(uint64_t value)
{
	char buff[MAX_BUFF_SIZE] = { 0x00 };
	sprintf(buff, "%lld", value);
	return buff;
}

API string strrm(const char* str, const char *rm_list)
{
	string ret;
	for (int n = 0; n < strlen(str); n++)
	{
		bool stat = true;
		for (int x = 0; x < strlen(rm_list); x++)
			if (str[n] == rm_list[x])
				stat = false;
		if (stat)
			ret += str[n];
	}
	return ret;
}

API string strreplace(string orig, string replace, string value)
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
				if (n + x < orig.size() + 1)
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

API string convert_process(const char *data)
{
	string fin;
	for (int n = 0; n < strlen(data); n++)
	{
		if (data[n] == '\\'&& n != strlen(data) - 1)
		{
			n++;
			switch (data[n])
			{
			case 't':
				fin += '\t'; break;
			case 'a':
				fin += '\a'; break;
			case 'b':
				fin += '\b'; break;
			case 'f':
				fin += '\f'; break;
			case 'n':
				fin += '\n'; break;
			case 'r':
				fin += '\r'; break;
			case 'v':
				fin += '\v'; break;
			case '\\':
				fin += '\\'; break;
			case '\'':
				fin += '\''; break;
			case '\"':
				fin += '\"'; break;
			case '\?':
				fin += '\?'; break;
			default:
				fin += data[n];
				break;
			}
		}
		else
			fin += data[n];
	}
	return fin;
}

API string f2s(float value)
{
	char buff[MAX_BUFF_SIZE];
	sprintf(buff, "%f", value);
	return buff;
}

API bool memequal(const void * a, const void * b, uint64_t size)
{
	const char *c = (const char*)a;
	const char *d = (const char*)b;
	for (int n = 0; n < size; n++)
		if (c[n] != d[n])
			return false;
	return true;
}

API vector<string> splite(string val,char spliter)
{
	vector<string>ret;
	int last = 0;
	for (int n=0;n<val.size();n++)
		if (val.at(n) == spliter)
		{
			ret.push_back(val.substr(last,n - last));
			last = n;
		}
	return ret;
}