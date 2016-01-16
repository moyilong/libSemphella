#pragma once
#include "main.h"


#define AES_KEY_ROW_NUMBER 4
#define AES_KEY_COLUMN_NUMBER 4
#define AES_ROUND_COUNT 10

class CAPI AES
{
public:
	AES(void);
	AES(unsigned char* key);
	~AES(void);

	void Encrypt(unsigned char *, unsigned char *, unsigned long long );
	void Decrypt(unsigned char *, unsigned char *, unsigned long long );

	void Encrypt(unsigned char *,  unsigned long long );
	void Decrypt(unsigned char *,  unsigned long long );

	void SetKey(unsigned char *key);

private:

	unsigned char swapbox[11][4][4];

	unsigned char* Cipher(unsigned char* input);
	unsigned char* InvCipher(unsigned char* input);

	unsigned char* Cipher(void * input, size_t length);
	unsigned char* InvCipher(void * input, size_t length);

	void KeyExpansion(unsigned char* key, unsigned char w[][4][AES_KEY_COLUMN_NUMBER]);
	unsigned char FFmul(unsigned char a, unsigned char b);

	void SubBytes(unsigned char state[][AES_KEY_COLUMN_NUMBER]);
	void ShiftRows(unsigned char state[][AES_KEY_COLUMN_NUMBER]);
	void MixColumns(unsigned char state[][AES_KEY_COLUMN_NUMBER]);
	void AddRoundKey(unsigned char state[][AES_KEY_COLUMN_NUMBER], unsigned char k[][AES_KEY_COLUMN_NUMBER]);

	void InvSubBytes(unsigned char state[][AES_KEY_COLUMN_NUMBER]);
	void InvShiftRows(unsigned char state[][AES_KEY_COLUMN_NUMBER]);
	void InvMixColumns(unsigned char state[][AES_KEY_COLUMN_NUMBER]);


};
