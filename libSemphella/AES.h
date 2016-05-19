///////////////////////////////
// http://mingcn.cnblogs.com //
//  xelz CopyRight (c) 2010  //
///////////////////////////////

#pragma once

#include "main.h"
#include "libSemphella.h"
#include "string.h"
#include "debug.h"
#include "crypt.h"

class CAPI AES
{
public:
	AES(unsigned char* key);
	AES();
	virtual ~AES();
	void SetPassword(unsigned char *key);

	void Decrypt(void *ptr, int len);
	void Crypt(void *ptr, int len);

private:
	unsigned char* Cipher(unsigned char* input);
	unsigned char* InvCipher(unsigned char* input);
	void* Cipher(void* input, int length = 0);
	void* InvCipher(void* input, int length);
	void AES_MP(void *ptr, int len, bool decrypt);
	void _AES_MP(void *ptr, int len, bool decrypt);

	unsigned char Sbox[256];
	unsigned char InvSbox[256];
	unsigned char w[11][4][4];

	void KeyExpansion(unsigned char* key, unsigned char w[][4][4]);
	unsigned char FFmul(unsigned char a, unsigned char b);

	void SubBytes(unsigned char state[][4]);
	void ShiftRows(unsigned char state[][4]);
	void MixColumns(unsigned char state[][4]);
	void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

	void InvSubBytes(unsigned char state[][4]);
	void InvShiftRows(unsigned char state[][4]);
	void InvMixColumns(unsigned char state[][4]);
};
