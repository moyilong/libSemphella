#include "algorthim.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#ifndef  __LINUX
#pragma comment(lib,"libeay32.lib")
#endif

FILE *key;
RSA *rsa;
int rsa_len;
unsigned char *p_de;
void ssl_password_prepare(string password)
{
	key = fopen(password.data(),"r");
	if (key == NULL)
	{
		cout << "RSA Open Key Faild!" << endl;
		exit(-1);
	}
	if (decrypt)
		rsa = PEM_read_RSAPrivateKey(key, NULL, NULL, NULL);
	else
		rsa = PEM_read_RSAPublicKey(key, NULL, NULL, NULL);
	if (rsa == NULL)
	{
		cout << "RSA Key Faild!" << endl;
		exit(-1);
	}
	rsa_len = RSA_size(rsa);
	p_de = (unsigned char *)malloc(rsa_len + 1);
	memset(p_de, 0, rsa_len);
}

void ssl_crypt_algrthom(char *data, int64_t len, int64_t bit)
{
	unsigned char *wback = (unsigned char *)malloc(len);
	memset(wback, 0, len);
	if (decrypt)
		RSA_private_decrypt(len, (unsigned char*)data, (unsigned char*)wback, rsa, RSA_NO_PADDING);
	else
		RSA_public_encrypt(len, (unsigned char*)data, wback, rsa, RSA_NO_PADDING);
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
		data[n] = (char)wback[n];
	free(wback);
}
uint64_t ssl_key_sum(string passwd)
{
	return 0;
}
ALGORTHIM ssl_aes(ssl_password_prepare, ssl_crypt_algrthom, getsumV2, 5,ssl_key_sum);