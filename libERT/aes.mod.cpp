#include "algorthim.h"
#include <libSemphella/AES.h>

AES ptr;
string stored;

void AES_INIT(string password)
{
	stored = password;
	ptr.SetPassword((unsigned char *)password.data());
}

void AES_CRYPT(char *data, int64_t len, int64_t bit)
{
	if (decryptmode)
	{
		ptr.Decrypt(data, len);
	}
	else {
		ptr.Crypt(data, len);
	}
}

ALGORTHIM aes_crypt(AES_INIT, AES_CRYPT, getsumV2, 8, "AES");