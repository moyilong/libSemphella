
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include "main_use.h"

__global__ void __CUDA_kernel_xor_crypt(char *data, int password_len, char *password)
{

}


char __CUDA_xor_crypt(string password, char *data, int len)
{
	char *pas;
	char *dat_h;
	cudaMalloc((void**)pas, password.size());
	cudaMalloc((void**)dat_h, len);
	cudaMemcpy(pas, password.data(),password.size(), cudaMemcpyHostToDevice);
	cudaMemcpy(dat_h, data,password.size(), cudaMemcpyHostToDevice);
	__CUDA_kernel_xor_crypt << <len, 1 >> >(dat_h, password.size(), pas);
	cudaThreadSynchronize();
	cudaMemcpy(data, dat_h, len, cudaMemcpyDeviceToHost);
}