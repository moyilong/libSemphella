#include "algorthim.h"

bool inited = false;

string _stored_pwd;
void CreateMatrix_NULL(string password)
{
	_stored_pwd = password;
}

void fastCrypt_ALGRTHOM(char *data, int64_t len, int64_t bit_off)
{
	fastCrypt(data, len, _stored_pwd);
}

void CryptAlgrthom(char *data, int64_t len, int64_t bit_off)
{
	xor_crypt(_stored_pwd, data, len);
}

void not_crypt(char *data, int64_t len, int64_t bit)
{
}

void mask_fun(char *data, int64_t len, int64_t bit)
{
	mask(data, len);
}
ALGORTHIM basic_old_alg(CreateMatrix_NULL, CryptAlgrthom, getsumV2, 2, "fastxor");
ALGORTHIM basic_fast(CreateMatrix_NULL, fastCrypt_ALGRTHOM, getsumV2, 6, "fastcrypt");
ALGORTHIM basic_none(CreateMatrix_NULL, not_crypt, getsumV2, 5, "copy");
ALGORTHIM basic_mask(CreateMatrix_NULL, mask_fun, getsumV2, 7, "mask");