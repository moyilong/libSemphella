#include <iostream>
#include <string>
#include <fstream>
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
	//char *cache = new char[sbuff.size()];
	char *cache = (char*)malloc(sbuff.size());
	strcpy(cache, sbuff.data());
	crypt(cache, sbuff.size(), password);
	string ret = cache;
	free(cache);
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
API void fastCrypt(char *data, int64_t len, string password, int PMLEN)
{
	char seed = 0;
	for (int n = 0; n < password.size(); n++)
		seed ^= password.at(n);
	char *pm = (char*)malloc(PMLEN);
#pragma omp parallel for
	for (int n = 0; n < PMLEN; n++)
	{
		pm[n] = n;
		for (uint64_t x = 0; x < password.size(); x++)
			pm[n] ^= password.at(x) + x + n + seed + PMLEN;
	}
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		char stuffix = n;
		for (int x = 0; x < PMLEN; x++)
			stuffix ^= pm[x] + (char)x + (char)n;
		data[n] ^= stuffix + len + seed;
	}
}

API void fcTest()
{
#define TEST_LEN 32
	char buff[TEST_LEN];
	char shadow[TEST_LEN];
	char decrypt[TEST_LEN];
	for (int n = 0; n < TEST_LEN; n++)
	{
		buff[n] = time(0) + rand();
		shadow[n] = buff[n];
	}
	fastCrypt(buff, TEST_LEN, "moyilong");
	char temp[16];
	string swap;
	cout << "Black:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", buff[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
	cout << endl << "White:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", shadow[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
	memcpy(decrypt, buff, TEST_LEN);
	fastCrypt(decrypt, TEST_LEN, "moyilong");
	cout << endl << "Decpt:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", decrypt[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
}

char *pre_cacl_tab = nullptr;
int64_t pct_len = 0;

inline char get_value(int64_t n)
{
	return n + 2;
}

API void mask(char *buff, int64_t len)
{
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		if (pre_cacl_tab != nullptr && len < pct_len)
			buff[n] ^= pre_cacl_tab[n];
		else
			buff[n] ^= get_value(n);
	}
}

API void pre_calc_pct(int64_t len)
{
	if (pct_len != 0 && pre_cacl_tab != nullptr)
	{
		pct_len = 0;
		if (pre_cacl_tab != nullptr)
			free(pre_cacl_tab);
	}
	pct_len = len;
	pre_cacl_tab = (char*)malloc(pct_len);
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
		pre_cacl_tab[n] = get_value(n);
}

#include "AES.h"

API void aesTest()
{
	AES ciper((unsigned char *)"moyilong");
#define TEST_LEN 128*8
	char buff[TEST_LEN];
	char shadow[TEST_LEN];
	char decrypt[TEST_LEN];
	for (int n = 0; n < TEST_LEN; n++)
	{
		buff[n] = time(0) + rand();
		shadow[n] = buff[n];
	}
	//fastCrypt(buff, TEST_LEN, "moyilong");
	ciper.Crypt(buff, TEST_LEN);
	char temp[16];
	string swap;
	cout << "Black:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", buff[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
	cout << endl << "White:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", shadow[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
	memcpy(decrypt, buff, TEST_LEN);
	//fastCrypt(decrypt, TEST_LEN, "moyilong");
	ciper.Decrypt(decrypt, TEST_LEN);
	cout << endl << "Decpt:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", decrypt[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
	long long count = 0;
	cout << endl;
	for (int n = 0; n < TEST_LEN; n++)
		if (decrypt[n] != shadow[n])
		{
			cout << "Different Finad @" << n << endl;
			count++;
		}
	cout << "All Find Different:" << count << endl;
}

API bool algrTest(crt_algr_func algr, int test_len)
{
#define TEST_LEN 128*8
	char buff[TEST_LEN];
	char shadow[TEST_LEN];
	char decrypt[TEST_LEN];
	for (int n = 0; n < TEST_LEN; n++)
	{
		buff[n] = time(0) + rand();
		shadow[n] = buff[n];
	}
	//fastCrypt(buff, TEST_LEN, "moyilong");
	algr(buff, TEST_LEN, "moyilong");
	char temp[16];
	string swap;
	cout << "Black:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", buff[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
	cout << endl << "White:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", shadow[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
	memcpy(decrypt, buff, TEST_LEN);
	//fastCrypt(decrypt, TEST_LEN, "moyilong");
	algr(buff, TEST_LEN, "moyilong");
	cout << endl << "Decpt:";
	for (int n = 0; n < TEST_LEN; n++)
	{
		sprintf(temp, "%+02x", decrypt[n]);
		cout << temp + strlen(temp) - 2 << " ";
	}
	long long count = 0;
	cout << endl;
	for (int n = 0; n < TEST_LEN; n++)
		if (decrypt[n] != shadow[n])
		{
			cout << "Different Finad @" << n << endl;
			count++;
		}
	cout << "All Find Different:" << count << endl;
}

string extoa(uint64_t val, const char *allow)
{
	int raidx = strlen(allow);
	uint64_t value = val;
	string ret;
	while (value > raidx)
	{
		if (value == 0)
			break;
		int xid = value%raidx;
		value = value / raidx;
		ret = allow[xid] + ret;
	}
	ret = allow[value] + ret;
	string xret;
	return ret;
}

API  vector<string> getsum2_decrypt(uint64_t sum, uint64_t begin_seek, uint64_t end_seek, string allow_string)
{
	vector<string>ret;
	int64_t mp_size = MAX_BUFF_SIZE*MAX_BUFF_SIZE;
	uint64_t value;
	char *buff;
	uint64_t ops = 0;
	double last_ops = 0, iops = 0;
	time_t begin = time(0);
	for (uint64_t n = begin_seek; n < end_seek; n += mp_size)
	{
		value = n;
#pragma omp parallel for
		for (int64_t p = 0; p < mp_size; p++)
		{
			string xval = extoa(value + p, allow_string.data()).data();
			if (getsumV2(xval.data(), xval.size()) == sum)
				ret.push_back(xval);
		}
		ops += mp_size;
		if (time(0) - begin > 0)
		{
			last_ops = iops;
			iops = ops / (time(0) - begin);
			cout << "iLen:" << extoa(value, allow_string.data()).size() << "\tPF:" << (long int)iops << "\tDIFF:" << last_ops - iops << "\tAF:" << ret.size() << "\t" << extoa(n, allow_string.data()) << "\n";
		}
	}
	return ret;
}

API bool dgst_check(const dgst dgst)
{
	if (dgst.main_data_verify != getsumV2(dgst.data, DGST_LEN))
		return false;
	return true;
}

API dgst dgst_calc(const char * data, int64_t len)
{
	dgst ret;
	for (int64_t n = 0; n < len; n++)
	{
		ret.data[get_n(DGST_LEN, n)] ^= data[n]+n;
	}
#pragma omp parallel for
	for (int n = 0; n < DGST_LEN; n++)
		ret.data[n] ^= data[get_n(len, n)]+n;
	ret.main_data_verify = getsumV2(ret.data, DGST_LEN);
	return ret;
}

API dgst dgst_merge(const dgst a, const dgst b)
{
	dgst ret;
#pragma omp parallel for
	for (int n = 0; n < DGST_LEN; n++)
		ret.data[n] = a.data[n] ^ b.data[n];
	ret.main_data_verify = getsumV2(ret.data, DGST_LEN);
	return ret;
}

API string dgst_string(const dgst a)
{
	string ret;
	for (int n = 0; n < DGST_LEN; n++)
	{
		ret += DEFAULT_WORD_BLACK_LIST[get_n(strlen(DEFAULT_WORD_BLACK_LIST), a.data[n])];
	}
	return ret;
}
