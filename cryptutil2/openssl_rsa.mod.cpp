#include "algorthim.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#ifndef  __LINUX
#pragma comment(lib,"libeay32.lib")
#endif

FILE *key;
RSA *rsa;
void ssl_password_prepare(string password)
{
	key = fopen(password.data(),"r");
	if (key == NULL)
	{
		cout << "RSA Open Key Faild!" << endl;
		exit(-1);
	}
	if (decrypt)
		 PEM_read_RSAPrivateKey(key, &rsa,NULL,NULL);
	else
		 PEM_read_RSAPublicKey(key,&rsa,NULL,NULL);
	if (rsa == NULL)
	{
		cout << "RSA Key Faild!" << endl;
		exit(-1);
	}
}

void ssl_crypt_algrthom(char *data, int64_t len, int64_t bit)
{
	char *wback = (char *)malloc(len);
	memset(wback, 0, len);
	if (decrypt)
		RSA_private_decrypt(len, (unsigned char*)data, (unsigned char*)wback, rsa, RSA_PKCS1_OAEP_PADDING);
	else
		RSA_public_encrypt(len, (unsigned char*)data, (unsigned char*)wback, rsa, RSA_PKCS1_OAEP_PADDING);
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
		data[n] = wback[n];
	free(wback);
}
uint64_t ssl_key_sum(string passwd)
{
	return 0;
}
ALGORTHIM ssl_aes(ssl_password_prepare, ssl_crypt_algrthom, getsumV2, 5,ssl_key_sum);