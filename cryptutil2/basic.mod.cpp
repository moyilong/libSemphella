#include "algorthim.h"

char **matrix;
bool inited = false;
void init_matrix()
{
	if (inited)
		return;
	matrix = (char**)malloc(sizeof(char*)*MATRIX_LEN);
#pragma omp parallel for
	for (int n = 0; n < MATRIX_LEN; n++)
		matrix[n] = (char*)malloc(sizeof(char*)*MATRIX_LEN);
	inited = true;
}

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

void LOW_MEM_ALGRTHOM(char *data, int64_t len, int64_t bit_off)
{
	xor_cryptV2_1_MATRIX_V2_LMEM(_stored_pwd, data, len, bit_off);
}

void M1(string pwd)
{
	init_matrix();
	CreateMatrix(pwd, matrix);
}

void M2(string pwd)
{
	init_matrix();
	CreateMatrixV2(pwd, matrix);
}

void xor_2_1(char *data, int64_t len, int64_t bit)
{
	xor_cryptV2_1(matrix, data, len, bit);
}

void xor_2(char *data, int64_t len, int64_t bit)
{
	xor_cryptV2(matrix, data, len, bit);
}

void not_crypt(char *data, int64_t len, int64_t bit)
{
}

void mask_fun(char *data, int64_t len, int64_t bit)
{
	mask(data, len);
}
#ifndef LOW_PERFORMANCE_DEVICE
ALGORTHIM basic_xor_V2_1(M1, xor_2_1, getsumV2, 0,"matrix alg V2_1 ");
ALGORTHIM basic_xor_V2(M1, xor_2, getsumV2, 1,"matrix alg V2");
ALGORTHIM basic_matrix_v2(M2, xor_2_1, getsumV2, 3,"matrix alg M2 V2_1");
#endif
ALGORTHIM basic_old_alg(CreateMatrix_NULL, CryptAlgrthom, getsumV2, 2,"Easy Xor Crypt");
ALGORTHIM basic_lowmem(CreateMatrix_NULL, LOW_MEM_ALGRTHOM, getsumV2, 4,"Matrix ALG V2_1 for Lowmem");
ALGORTHIM basic_fast(CreateMatrix_NULL, fastCrypt_ALGRTHOM, getsumV2, 6,"fastCrypt");
ALGORTHIM basic_none(CreateMatrix_NULL, not_crypt, getsumV2, 5,"only copy");
ALGORTHIM basic_mask(CreateMatrix_NULL, mask_fun, getsumV2, 7,"only mask");