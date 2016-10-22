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

uint64_t mpSum_HAL(const char *data, int64_t len)
{
	return mpsum2(data, len);
}
uint64_t mpSum1_HAL(const char *data, int64_t len)
{
	uint64_t ret;
	emmx get=mpSum(data, len,sizeof(uint64_t));
	memcpy(&ret, get.ptr,sizeof(uint64_t));
	return ret;
}
ALGORTHIM basic_old_alg(CreateMatrix_NULL, CryptAlgrthom, getsumV2, 2, "fastxor");
ALGORTHIM basic_fast(CreateMatrix_NULL, fastCrypt_ALGRTHOM, getsumV2, 6, "fastcrypt");
ALGORTHIM basic_none(CreateMatrix_NULL, not_crypt, getsumV2, 5, "copy");
ALGORTHIM basic_mask(CreateMatrix_NULL, mask_fun, getsumV2, 7, "mask");
ALGORTHIM basic_fast_x(CreateMatrix_NULL, fastCrypt_ALGRTHOM, mpSum_HAL, 9, "fcpt_fsum");
ALGORTHIM basic_fast_l(CreateMatrix_NULL, fastCrypt_ALGRTHOM, mpSum1_HAL, 10, "fcpt_lsum");