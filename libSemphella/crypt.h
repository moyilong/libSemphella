#pragma once

API char get_off();

#ifndef _EL_CPYOFF
#define _EL_CPYOFF  0x5F
#endif

#ifndef DEFAULT_WORD_WHITE_LIST
#define DEFAULT_WORD_WHITE_LIST "UVyZQRSgBcde1Cz5qrsFGHT3b2nopit4xWXYauKLMAm690vwNOPjklh78DEfIJ"
#endif

#ifndef DEFAULT_WORD_BLACK_LIST
#define DEFAULT_WORD_BLACK_LIST "MUsXGH1bEgTYijNwdklm7ZIJaxSAVWPQtn2340FfOCDuvh8yz56cBoR9qrepKL"
#endif

API char xbit(const char *data,long long len,const char off=_EL_CPYOFF);
API void crypt(char* data,long long len,const char *password);



API void word_list_convert( char *str,const char *word_white_list=DEFAULT_WORD_WHITE_LIST
	, const char *word_balck_list = DEFAULT_WORD_BLACK_LIST);


API string word_crypt(const string origin, const string password);
API bool word_list_check(const char *word_white_list,const  char *word_black_list);
API void crypt_t(char *data, long long len,const char *password, const char *la= DEFAULT_WORD_WHITE_LIST, const char *lb= DEFAULT_WORD_BLACK_LIST);


API void dymanic_code(const char *license, long long license_len, string password, string &ret);

//inline feature

inline void crypt_t(char *data, long long len, string password, const char *la= DEFAULT_WORD_WHITE_LIST, const char *lb= DEFAULT_WORD_BLACK_LIST)
{
	crypt_t(data, len, password.data(), la, lb);
}
inline void word_list_deconvert(char *str, const char *word_white_list = DEFAULT_WORD_WHITE_LIST
	, const char *word_balck_list = DEFAULT_WORD_BLACK_LIST)
{
	word_list_convert(str, word_balck_list, word_white_list);
};

inline void crypt(char *data, long long len, const string password)
{
	crypt(data, len, password.data());
}