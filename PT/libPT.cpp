#include <iostream>
#include <string>
using namespace std;
#include <math.h>
#include <omp.h>
#include <libSemphella/libSemphella.h>
#include "libPT.h"

#ifndef MATRIX_SIZE
#define MATRIX_SIZE	 64
#endif
#define U_RANDOM	0xA55B99FC

inline  char c_char(int a, int b, int c, int d)
{
	char ret=U_RANDOM;
	ret += a + b + c + d;
	ret *= a + b - c*d;
	ret = sin(sqrt(ret))*pow(a, b) - sqrt(d) * U_RANDOM;
	ret = sin(ret);
	ret = log10(ret);
	return sin(cos(tan(tan(cos(ret)))));

}



inline  double c_double(double a, double b, double c, double d)
{
	double ret=U_RANDOM;
	ret += a + b + c + d;
	ret *= a + b - c*d;
	ret = sin(sqrt(ret))*pow(a,b)-sqrt(d) * U_RANDOM ;
	ret = sin(ret);
	ret = log10(ret);
	return sin(cos(tan(tan(cos(ret)))));
}

inline long long c_long(long long a, long long b, long long c, long long d)
{
	long long ret = U_RANDOM;
	ret += a + b + c + d;
	ret *= a + b - c*d;
	ret = sin(sqrt(ret))*pow(a, b) - sqrt(d) * U_RANDOM;
	ret = sin(ret);
	ret = log10(ret);
	return sin(cos(tan(tan(cos(ret)))));
}

 PT_TIME PT()
{
	PT_TIME ret;
	char ****matrix;
	long long ****l_matrix;
	double ****d_matrix;
	ret.start = clock();
	matrix = (char****)malloc(sizeof(char*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		matrix[a] = (char***)malloc(sizeof(char*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			matrix[a][z] = (char**)malloc(sizeof(char*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			for (int y = 0; y < MATRIX_SIZE;y++)
				matrix[a][z][y] = (char*)malloc(sizeof(char)*MATRIX_SIZE);
	l_matrix = (long long ****)malloc(sizeof(long long*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		l_matrix[a] = (long long***)malloc(sizeof(long long*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			l_matrix[a][z] = (long long**)malloc(sizeof(long long*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			for (int y = 0; y < MATRIX_SIZE; y++)
				l_matrix[a][z][y] = (long long*)malloc(sizeof(long long)*MATRIX_SIZE);
	d_matrix = (double****)malloc(sizeof(double*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		d_matrix[a] = (double***)malloc(sizeof(double*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			d_matrix[a][z] = (double**)malloc(sizeof(double*)*MATRIX_SIZE);
#pragma omp parallel for
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			for (int y = 0; y < MATRIX_SIZE; y++)
				d_matrix[a][z][y] = (double*)malloc(sizeof(double)*MATRIX_SIZE);
	ret.mem_alloc = clock();

#pragma omp parallel for 
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			for (int y = 0; y < MATRIX_SIZE; y++)
				for (int x = 0; x < MATRIX_SIZE; x++)
					matrix[a][z][y][z] = c_char(a, z, y, x);
	//cout << "char caculate complete!" << endl;
	ret.caculate_c_time = clock();
#pragma omp parallel for 
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			for (int y = 0; y < MATRIX_SIZE; y++)
				for (int x = 0; x < MATRIX_SIZE; x++)
					l_matrix[a][z][y][z] = c_long(a, z, y, x);
	//cout << "long long caculate complete!" << endl;
	ret.caculate_l_time = clock();
#pragma omp parallel for 
	for (int a = 0; a < MATRIX_SIZE; a++)
		for (int z = 0; z < MATRIX_SIZE; z++)
			for (int y = 0; y < MATRIX_SIZE; y++)
				for (int x = 0; x < MATRIX_SIZE; x++)
					d_matrix[a][z][y][z] = c_double(a, z, y, x);
	//cout << "double caculate complete!" << endl;
	ret.caculate_d_time = clock();
	int stop = pow(MATRIX_SIZE, 4);
#pragma omp parallel for 
	for (int n = 0; n < stop; n++)
		rand();
	ret.random = clock();
	return ret;
	free(matrix);
	free(l_matrix);
	free(d_matrix);
}
