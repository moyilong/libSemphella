#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>
using namespace std;
#include "libSemphella.h"
#include "string.h"

API int strfind(const char *str,char find,bool wn)
{
    int ret=0;
	for (int n = 0; n < strlen(str); n++)
		if (str[n] == find)
			if (wn)
				return n;
			else
				ret++;
    return ret;
}



CAPI void strcpy(char *dest,const char *origin,long long cplen,long long r_off,long long w_off)
{
#pragma omp parallel for
    for (long long n=0;n<cplen;n++)
        dest[n+w_off]=origin[n+r_off];
}


API string strreplace(const char*origin, const char *replace, const char*value)
{
	string ret;
	if (strlen(origin) < strlen(replace))
	{
		ret = origin;
		return ret;
	}
	for (int n = 0; n < strlen(origin); n++)
		if (origin[n] == replace[0] && strlen(origin) - n >= strlen(replace))
		{
		bool check = true;
		for (int x = 0; x < strlen(replace); x++)
			if (replace[x] != origin[n + x])
				check = false;
		if (check)
		{
			n += strlen(replace)-1;
			ret += value;
		}
		else
			ret += origin[n];
		}
		return ret;
}

API bool streval(const char *a, const char *b)
{
	if (strlen(a) != strlen(b))
		return false;
	for (int n = 0; n < strlen(a); n++)
		if (a[n] != b[n])
			return false;
	return true;
}

API char* eitoa(int num, char*str, int radix,const char *word_list)
{
	char *index = (char*)calloc(strlen(word_list), sizeof(char));
	strcpy(index, word_list);
	unsigned unum;
	int i = 0, j, k;

	if (radix == 10 && num<0)
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
	free(index);
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

API string upper_string(string str,bool upper)
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