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

API int strfind(const char *str,char find)
{
    int ret=0;
    for (int n=0;n<strlen(str);n++)
        if (str[n]==find)
            ret++;
    return ret;
}

API void strcpy(char *dest,const char *origin,long long cplen,long long r_off,long long w_off)
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
		else{
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
	char index[sizeof(DEFAULT_WORD_WHITE_LIST)];
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
	return str;
}
