#include "license.h"
#include "string.h"
#include "debug.h"
#include <math.h>
libDebug lics("License");
const char ASCODE[] = DEFAULT_WORD_WHITE_LIST;

inline unsigned int fast_rand(int value, int max)
{
	int ret = sin(value)*max;
	return abs(ret);
}

license::license()
{
	memset(data, 0, sizeof(data));
	memset(sum, 0, sizeof(sum));
}


license::~license()
{
	memset(data, 0, sizeof(data));
	memset(sum, 0, sizeof(sum));
}

license::license(string cn, string na, string tx)
{
	create(cn, na, tx);
}

license::license(const char *xdata, int len)
{
	create(xdata, len);
}

void license::create(string cn, string na, string tx)
{
	string str = cn + na + tx;
	char pdata[LICENSE_LEN];
#pragma omp parallel for
	for (int n = 0; n < LICENSE_LEN; n++)
		pdata[n] = str.at(fast_rand(n+rand(), str.size())) ^ str.at(fast_rand(n+str.size()+rand(), str.size())) + rand();
	create(pdata, LICENSE_LEN);
}
void license::create(const char * xdata, int len)
{
#pragma omp parallel for
	for (int n = 0; n < LICENSE_LEN; n++)
	{
		char cl = n;
		for (int x = 0; x < len; x++)
			cl += xdata[fast_rand(n + x+rand(), len)];
		data[n] = ASCODE[fast_rand(cl+rand()+n,strlen(ASCODE))];
	}
	char sumd[CHECK_LEN];
	caculate_sum(data, sumd);
#pragma omp parallel for
	for (int n = 0; n < CHECK_LEN; n++)
		sum[n] = sumd[n];
}

void license::load(string import)
{
	char pdata[LICENSE_LEN];
	char psum[CHECK_LEN];
	strcpy(pdata, 0, LICENSE_LEN,0,0);
	strcpy(psum, 0, CHECK_LEN, LICENSE_LEN, 0);
	char csum[CHECK_LEN];
	caculate_sum(pdata, csum);
	bool stat = true;
#pragma omp parallel for
	for (int n = 0; n < CHECK_LEN; n++)
		if (psum[n] != csum[n])
			stat = false;
	if (!stat)
	{
		cout << "Check Faild!" << endl;
		return;
	}
	strcpy(data, pdata, LICENSE_LEN,0,0);
	strcpy(sum, csum, CHECK_LEN, 0, 0);
}
void license::caculate_sum(const char * origin, char * wsum)
{
#pragma omp parallel for
	for (int n = 0; n < CHECK_LEN; n++)
	{
		wsum[n] = data[fast_rand(data[fast_rand(n, LICENSE_LEN)] + n, LICENSE_LEN)];
		char vdata = ASCODE[fast_rand(wsum[n], strlen(ASCODE))];
		//if (n % 5 == 0)
		//	lics << "SumCaculate:" << wsum[n] << "->" << data << endl;
		wsum[n] = vdata;
	}
	wsum[CHECK_LEN - 1] = '\0';
}

license::license(string import)
{
	load(import);
}

string license::dump()
{
	caculate_sum(data, sum);
	string ret;
	ret = data;
	ret += sum;
	return ret;
}

void license::crypt(char * cdata, int64_t size)
{
#pragma omp parallel for
	for (int64_t n = 0; n < size; n++)
	{
		int a = fast_rand(LICENSE_LEN, n);
		int b = fast_rand(CHECK_LEN, data[a]);
		int c = data[fast_rand(LICENSE_LEN, a^b)] ^ sum[fast_rand(CHECK_LEN, a + b)];
		cdata[n] ^=c;		
	}
}
