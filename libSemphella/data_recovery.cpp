#include "data_recovery.h"
#include "crypt.h"
#include <limits>
struct REC_HEAD {
	char a = 'S';
	uint64_t checksum;
	int64_t block_len;
	char b = 'E';
};

API int64_t get_block_len(int64_t data_len)
{
	return (data_len / 2) + sizeof(REC_HEAD);
}

struct _TBLOCK {
	char a;
	char b;
	char c;
};

vector<_TBLOCK> cache_data;
char CACHE_STAT = -1;

API void init_cache()
{
#undef min 
#undef max
	const int char_min = numeric_limits<char>::min();
	const int char_max = numeric_limits<char>::max();

	if (CACHE_STAT != 1)
	{
		if (CACHE_STAT == -1)
		{
			CACHE_STAT = 0;
			for (int x = char_min; x < char_max; x++)
				for (int y = char_min; y < char_max; y++)
				{
					_TBLOCK temp = {
						x,y,x^y
					};
					cache_data.push_back(temp);

				}
			CACHE_STAT = 1;
		}
		while (CACHE_STAT == 1)
			esleep(1);
	}
}

STAT try_block(const char a, const char b, const char c, char &wa, char &wb)
{
	if (a^b == c)
		return OK;
	init_cache();
	vector<_TBLOCK> maybe;
#pragma omp parallel for
	for (int n = 0; n < cache_data.size(); n++)
	{
		if (cache_data.at(n).c == c &&
			(cache_data.at(n).a == a || (cache_data.at(n).b == b)))
			maybe.push_back(cache_data.at(n));
	}
	if (maybe.size() > 1)
		return MultiMatch;
	else
		if (maybe.size() == 0)
			return NoMatch;
	wa = maybe.at(0).a;
	wb = maybe.at(0).b;
	return Fixed;

}

API void CaculateRecovery(const char * data, int64_t len, char * ret)
{
	int64_t ret_len = get_block_len(len);
	int64_t splite = len / 2;
#pragma omp parallel for
	for (int64_t n = 0; n < splite; n++)
	{
		ret[n + sizeof(REC_HEAD)] = data[n] ^ data[n + splite];
	}
	REC_HEAD head;
	head.block_len = len;
	head.checksum = getsumV2(ret + sizeof(REC_HEAD), splite);
	memcpy(ret, &head, sizeof(REC_HEAD));

}

API STAT Recovery(const char * data, int64_t len, const char * rec)
{
	STAT ret = OK;
	REC_HEAD head;
	memcpy(&head, rec, sizeof(REC_HEAD));
	int64_t splite = len / 2;
	if (getsumV2(rec + sizeof(REC_HEAD), splite) != head.checksum)
		return OK;
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		if (ret != OK)
		{
			if (rec[n + sizeof(REC_HEAD)] != data[n] ^ data[n + splite])
			{
				ret = FAILD;
				char a, b, c;
				STAT stat = try_block(data[n], data[n + splite], rec[n + sizeof(REC_HEAD)], a, b);
				switch (stat)
				{
				case NoMatch:
				case MultiMatch:
					ret = stat;
					break;
				case Fixed:
					break;
				}

			}
		}
	}
	return ret;
}

API STAT VerifyRecoveryData(const char * rec, int64_t len)
{
	REC_HEAD head;
	memcpy(&head, rec, sizeof(REC_HEAD));
	int64_t splite = len / 2;
	if (getsumV2(rec + sizeof(REC_HEAD), splite) != head.checksum)
		return FAILD;
	return OK;
}
