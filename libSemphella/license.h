#pragma once
#include "libSemphella.h"
#include "crypt.h"

#define LICENSE_LEN	4096
#define CHECK_LEN	LICENSE_LEN/4

#define DUMP_LEN	(LICENSE_LEN)+(CHECK_LEN)

class CAPI license
{
public:
	license();
	license(string cn, string na, string tx);
	license(const char *xdata, int len);
	license(string import);
	~license();
	string dump();
	void crypt(char *data, int64_t size);
	void caculate_sum(const char *origin, char *wsum);
	void load(string import);
	void create(string cn, string na, string tx);
	void create(const char *xdata, int len);
protected:
	char data[LICENSE_LEN];
	char sum[CHECK_LEN];
};

