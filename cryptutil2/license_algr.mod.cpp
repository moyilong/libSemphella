#include "algorthim.h"
#include <libSemphella/license.h>

	LICENSE lic;
	void LoadLicense(string license)
	{
		lic = license;
	}

	uint64_t pwd_sum(string license)
	{
		return getsumV2(lic.main.data(), lic.main.size());
	}

	void LCrypt(char *data,int64_t len,int64_t bit)
	{
#pragma omp parallel for
		for (int n = 0; n < len; n++)
			data[n] = data[n] ^ ArgmentGetValue(lic, n, bit);
	}

	ALGORTHIM LIC(LoadLicense, LCrypt, getsumV2, 6, pwd_sum);