#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>
#include <math.h>
using namespace std;
#include "libSemphella.h"
#include "crypt.h"
#include "math.h"
#include "debug.h"
#include "string.h"
//libDebug cpt("Semphella-CryptMod");

#define cpt debug<<"[CryptModules]"

__inline int get_n(int max, int n)
{
	//return abs(sin(max)*n);
	double ret = 0;
	ret = sin(n)*max;
	//ret = abs(ret);
	if (ret < 0)
		ret = -ret;
	return ret;
}

API uint64_t getsumV2(const char *data, int64_t len)
{
	uint64_t dr = 0;
	uint64_t ret = 0;
	for (int x = 0; x < len; x++)
		dr += data[x] + x;
	for (int64_t n = 0; n < len; n++)
	{
		char now = data[n];
		char nx;
		if (n == 0)
			nx = data[1];
		else
			nx = data[n - 1];
		char bc = now + nx;
		char lc = now - nx;
		now *= 0xFF;
		nx *= 0xFFFF;
		bc *= 0xFFFFFF;
		lc *= 0xFFFFFFFF;
		ret += (now + nx + nx ^ data[n] + now ^ len) + data[n] * 0x5dd96aff;
		ret += now + nx + lc + bc;
		ret = (ret << 8) ^ dr + n;
	}
	return ret;
}

API char xor_crypt(string password, char *data, int len)
{
#pragma omp parallel for
	for (int n = 0; n < len; n++)
	{
		int value = sin(n)*password.size();
		value = abs(value);
		data[n] = data[n] ^ (~password.at(value) + n);
	}
}

API void CreateMatrix(string password, char **matrix)
{
#pragma omp parallel for
	for (int64_t x = 0; x < MATRIX_LEN; x++)
		for (int64_t y = 0; y < MATRIX_LEN; y++)
		{
			int value = sin(x + y)*password.size();
			//			value = abs(value);
			if (value < 0)
				value = -value;
			matrix[x][y] = password.at(value) + ~(value - x - y) + x - y;
		}
}
API void CreateMatrixV2(const char * data, size_t dlen, char ** matrix)
{
#pragma omp parallel for
	for (int x = 0; x < MATRIX_LEN; x++)
		for (int y = 0; y < MATRIX_LEN; y++)
		{
			int xa = x^y;
			int xb = x | y;
			int xc = x & y;
			int xd = x*y;
			int xma = (xa + xb - xc + xd) ^ (xa >> 8 + xb << 8 + xc >> 4 + xd << 4);
			matrix[x][y] = data[get_n(dlen, xma ^ xa + xd)] ^ xma >> 8 - x + y;
			matrix[x][y] = matrix[x][y] ^ xa ^ xb;
			matrix[x][y] = matrix[x][y] >> 8;
		}
}
API void xor_cryptV2(char **matrix, char *data, int64_t len, int64_t bit_off)
{
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		int x = sin(n)*MATRIX_LEN;
		int y = sin(~n)*MATRIX_LEN;
		x = abs(x);
		y = abs(y);
		data[n] = data[n] ^ (matrix[x][y] + n + bit_off);
	}
}
API void xor_cryptV2_1(char **matrix, char *data, int64_t len, int64_t bit_off)
{
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		int x = sin(n + bit_off)*MATRIX_LEN;
		int y = sin(~(n + bit_off))*MATRIX_LEN;
		x = abs(x);
		y = abs(y);
		data[n] = data[n] ^ (matrix[x][y] + n + bit_off);
	}
}
API void xor_cryptV2_1_MATRIX_V2_LMEM(string password, char * data, int64_t len, int64_t bitoff)
{
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		int x = sin(n + bitoff)*MATRIX_LEN;
		int y = sin(~(n + bitoff))*MATRIX_LEN;
		x = abs(x);
		y = abs(y);
		char matrix_value = 0;
		int xa = x^y;
		int xb = x | y;
		int xc = x & y;
		int xd = x*y;
		int xma = (xa + xb - xc + xd) ^ (xa >> 8 + xb << 8 + xc >> 4 + xd << 4);
		matrix_value = data[get_n(password.size(), xma ^ xa + xd)] ^ xma >> 8 - x + y;
		matrix_value = matrix_value^xa ^xb;
		matrix_value = matrix_value >> 8;
		data[n] = data[n] ^ (matrix_value + n + bitoff);
	}
}
inline char xbit(const char *data, long long len, const char off)
{
	char ret = off;
	for (long long n = 0; n < len; n++)
		ret += data[n] + off;
	return ret;
}

CAPI void crypt(char* data, long long len, const char *password)
{
#pragma omp parallel for
	for (long long n = 0; n < len; n++)
		data[n] ^= password[get_n(strlen(password), n)] + len - n;
}

CAPI void crypt_t(char *data, long long len, const char *password, const char *la, const char *lb)
{
	char *t_password = (char*)malloc(strlen(password));
	strcpy(t_password, password, 0, 0, 0);
	word_list_convert(data, la, lb);
	crypt(data, len, t_password);
	free(t_password);
}

API bool word_list_check(const char *word_white_list, const char *word_black_list)
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

CAPI void word_list_convert(char *str, const char *word_white_list, const char *word_balck_list)
{
	if (!word_list_check(word_white_list, word_balck_list))
		cpt << "Warring:word list dict is unsecure" << endl;
#pragma omp parallel for
	for (int n = 0; n < strlen(str); n++)
	{
		char swap = str[n];
		for (int y = 0; y < strlen(word_white_list); y++)
			if (word_white_list[y] == str[n])
			{
				swap = word_balck_list[y];
				break;
			}
		str[n] = swap;
	}
}

API string word_encrypt(const string origin, const string password)
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
		string io = origin.substr(n, n + 2);
		sbuff += atoi(io.data());
	}
	char *cache = new char[sbuff.size()];
	strcpy(cache, sbuff.data());
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
		eitoa(now^c, get, 16, DEFAULT_WORD_BLACK_LIST);
		cpt << "GetD:" << get << endl;
		string temp = get;
		//ret += get[strlen(get) - 1];
		//ret += get[strlen(get) - 2];
		ret += temp.substr(temp.size() - 2);
	}
}
API void dymanic_code(const char *license, long long license_len, string password, string &ret)
{
	time_t ntime = time(0);
	count_t now = ntime / 10;
	now += now * 10;
	now += now - 'm';
	cpt << "get time now:" << now << endl;
	half_dymanic_code(license, license_len, password, ret, now);
}

API float getsum(const char *data, int len)
{
	float ret = 0;
#pragma omp parallel for reduction(+:ret)
	for (int n = 0; n < len; n++)
	{
		ret += log(data[n] - n + len*sin(data[n]) - cos(data[n]) - log10(data[n]));
		if (data[n] % 2 == 0)
		{
			ret += 2 * sin(data[n]);
		}
		else
			ret -= 2 * sin(data[n]);
	}
	return ret;
}

#include "sum.h"

API uint64_t md5_SUM(const char *data, uint64_t len)
{
	unsigned char result[ZEN_MD5_HASH_SIZE];
	memset(result, 0, sizeof(result));
	ZEN_LIB::md5((const unsigned char *)data, len, result);
	return getsumV2((char*)result, ZEN_MD5_HASH_SIZE);
}

API uint64_t sha1_SUM(const char *data, uint64_t len)
{
	unsigned char result[ZEN_SHA1_HASH_SIZE];
	memset(result, 0, sizeof(result));
	ZEN_LIB::sha1((const unsigned char *)data, len, result);
	return getsumV2((char*)result, ZEN_SHA1_HASH_SIZE);
}
#define PMLEN	4
API void fastCrypt(char *data, uint64_t len, string password)
{
	char password_mutex[PMLEN];
	for (int n = 0; n < PMLEN; n++)
	{
		password_mutex[n] = 0;
#pragma omp parallel for
		for (int x = 0; x < password.size(); x++)
			password_mutex[n] = (password_mutex[n] << 8) + password.at(x) ^ n;
	}
	uint64_t value_data = getsumV2(password.data(), password.size()) ^ getsumV2(password_mutex, PMLEN);
#undef max
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		char seek = len;
		for (int x = 0; x < PMLEN; x++)
			seek = (~seek << 4) + password_mutex[x] ^ (n ^ (~value_data));
		data[n] = data[n] ^ seek;
	}
}