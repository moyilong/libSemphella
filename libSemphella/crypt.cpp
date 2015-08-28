
#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>
using namespace std;
#include "libSemphella.h"
#include "crypt.h"
#include "math.h"
#include "debug.h"
#include "string.h"
libDebug cpt("Semphella-CryptMod");

API inline char xbit(const char *data,long long len,const char off)
{
    char ret=off;
    for (long long n=0;n<len;n++)
		ret += data[n]+off;
    return ret;
}


API void crypt(char* data,long long len,const char *password)
{
#pragma omp parallel for
    for (long long n=0;n<len;n++)
        data[n]^=xbit(password,strlen(password),n+strlen(password)+_EL_CPYOFF);
}

API void crypt_t(char *data, long long len,const char *password, const char *la, const char *lb)
{
	char *t_password = (char*)malloc(strlen(password));
	strcpy(t_password, password, 0, 0, 0);
	word_list_convert(data,la,lb);
	crypt(data, len, t_password);
	free(t_password);
}

API bool word_list_check(const char *word_white_list,const char *word_black_list)
{
	cpt << "Start Check Key" << endl;
	if (strlen(word_white_list) != strlen(word_black_list))
	{
		cpt << "Error: find a pair of none equal length key" << endl;
#ifndef __ALLOW_NONE_EQUAL_LENGHT_LIST_DICT
		abort();
#endif
	}
	bool ret = true;
//#pragma omp parallel for 
	for (int n = 0; n < strlen(word_white_list); n++)
		if (word_white_list[n] != word_white_list[n])
			ret = false;
	if (!ret)
	{
		cpt << "WordList Checker find a pair of unsecure key" << endl;
#ifndef __ALLOW_UNSECURE_KEY
		abort();
#endif
	}
	else {
		cpt << "Check Final key not have problem!" << endl;
	}
	return ret;
}

API void word_list_convert(char *str, const char *word_white_list, const char *word_balck_list)
{
	if (!word_list_check(word_white_list, word_balck_list))
		cpt << "Warring:word list dict is unsecure" << endl;
#pragma omp parallel for
    for (int n=0;n<strlen(str);n++)
    {
        char swap=str[n];
        for (int y=0;y<strlen(word_white_list);y++)
			if (word_white_list[y] == str[n])
			{
			swap = word_balck_list[y];
			break;
			}
        str[n]=swap;
    }
}

API string word_encrypt(const string origin,const string password)
{
	char *buff = new char[origin.size()];
	strcpy(buff, origin.data());
	crypt(buff, origin.size(), password);
	string ret;
	for (long long n = 0; n < origin.size(); n++)
	{
		char cbuff[MAX_BUFF_SIZE];
		sprintf(cbuff, "%x", buff[n]);
		string xbuff = cbuff;
		ret += xbuff.substr(xbuff.size() - 2);
	}
	return ret;
}

API string word_decrypt(const string origin, const string password)
{
	string sbuff;
	for (int n = 0; n < origin.size(); n += 2)
	{
		string io = origin.substr(n, n+2);
		sbuff+=atoi(io.data());
	}
	char *cache = new char[sbuff.size()];
	strcpy(cache,sbuff.data());
	crypt(cache, sbuff.size(), password);
	return cache;
}

bool cached = false;
char cache;

API char get_off()
{
	if (!cached)
	{
		cached = true;
		word_list_check(DEFAULT_WORD_BLACK_LIST, DEFAULT_WORD_WHITE_LIST);
		for (int n = 0; n < strlen(DEFAULT_WORD_WHITE_LIST); n++)
			cache ^= DEFAULT_WORD_BLACK_LIST[n] ^ DEFAULT_WORD_WHITE_LIST[n];
	}
	return cache;
}


API void half_dymanic_code(const char *license, long long license_len, string password, string &ret, count_t now)
{
	string license_buff;
	for (int n = 0; n < license_len; n++)
	{
		char buff[MAX_BUFF_SIZE];
		sprintf(buff, "%d", license[n]);
		license_buff += license_buff;
	}
	char crypt_len[MAX_BUFF_SIZE] = { now };
	crypt(crypt_len, MAX_BUFF_SIZE, license_buff);
	crypt(crypt_len, MAX_BUFF_SIZE, password);
	char lbuff[MAX_BUFF_SIZE] = { 0x00 };
	eitoa(now, lbuff, 16);
	crypt(crypt_len, MAX_BUFF_SIZE, lbuff);
	cpt << "pre process data complete!" << endl;
	for (int n = 0; n < 6; n++)
	{
		for (int b = 0; b < MAX_BUFF_SIZE; b++)
			crypt_len[n] += now + b - n;
		char get[MAX_BUFF_SIZE];
		char c;
		c = xbit(crypt_len, MAX_BUFF_SIZE, !now + !n) + n * 100;
		eitoa(now^c, get, 16,DEFAULT_WORD_BLACK_LIST);
		cpt << "GetD:" << get << endl;
		string temp = get;
		//ret += get[strlen(get) - 1];
		//ret += get[strlen(get) - 2];
		ret += temp.substr(temp.size() - 2);
	}
}
API void dymanic_code(const char *license, long long license_len,string password,string &ret)
{
	time_t ntime = time(0);
	count_t now = ntime / 10;
	now += now * 10;
	now += now - 'm';
	cpt << "get time now:" << now << endl;
	half_dymanic_code(license, license_len, password, ret,now);
}