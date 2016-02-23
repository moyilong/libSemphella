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

uint64_t normally_status()
{
	return 0;
}



ALGORTHIM basic_xor_V2_1(M1, xor_2_1, getsumV2,0);
ALGORTHIM basic_xor_V2(M1, xor_2, getsumV2,1);
ALGORTHIM basic_old_alg(CreateMatrix_NULL, CryptAlgrthom, getsumV2, 2);
ALGORTHIM basic_matrix_v2(M2, xor_2_1, getsumV2, 3);
ALGORTHIM basic_lowmem(CreateMatrix_NULL, LOW_MEM_ALGRTHOM, getsumV2, 4);
