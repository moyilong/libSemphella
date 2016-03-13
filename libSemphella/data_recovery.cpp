#include "data_recovery.h"
#include "crypt.h"
#include <limits>

#define STR "RECDATA_BEG"
#define END	"RECDATA_END"


struct REC_HEAD {
	char a[sizeof(STR)];
	uint64_t checksum;
	int x = 0;
	int y = 0;
	char b[sizeof(END)];
	inline REC_HEAD()
	{
		strcpy(a, STR);
		strcpy(b, END);
	}
};

void GetN(int64_t len, REC_HEAD &head)
{
	int div = 1;
	for (int x = 64; x > 0; x++)
		if (len %x == 0)
		{
			div = x;
			break;
		}
	head.x = div;
	head.y = len / div;
	debug << "Divison " << len << " Return " << head.x << " * " << head.y << endl;
}

API int64_t get_block_len(int64_t data_len)
{
	REC_HEAD head;
	GetN(data_len, head);
	int64_t le=sizeof(REC_HEAD) + head.x + head.y+1;
	debug << "Return Len:" << le << endl;
	return le;
}



API void CaculateRecovery(const char * data, int64_t len, char * ret)
{
	REC_HEAD head;
	GetN(len, head);
	char *fix_data = (char*)malloc(sizeof(char)*(head.x + head.y));
#pragma omp parallel for
	for (int n = 0; n < head.x; n++)
	{
		fix_data[n] = 0;
		for (int y = 0; y < head.y; y++)
			fix_data[n] ^= data[head.y*n + y];
	}
#pragma omp parallel for
	for (int n = 0; n < head.y ; n++)
	{
		fix_data[n+head.x] = 0;
		for (int x = 0; x < head.x; x++)
			fix_data[n+head.x] ^= data[head.x*n + x];
	}
	memcpy(ret + sizeof(REC_HEAD), fix_data, sizeof(char)*(head.x + head.y));
	head.checksum = getsumV2(ret + sizeof(REC_HEAD), get_block_len(len)-sizeof(REC_HEAD));
	memcpy(ret, &head, sizeof(REC_HEAD));
	ret[get_block_len(len) - 1] = 'B';
}

API STAT Recovery(char * data, int64_t len, const char * rec)
{
	REC_HEAD head;
	memcpy(&head, rec, sizeof(REC_HEAD));
	char *read_data = (char*)malloc(sizeof(char)*(head.x + head.y));
	memcpy(read_data, rec + sizeof(REC_HEAD), head.x + head.y);
	int warring = -1;
	bool stat_ok = true;
#pragma omp parallel for
	for (int n = 0; n < head.x; n++)
	{
		char bit = 0;
		for (int x = 0; x < head.x; x++)
			for (int y = 0; y < head.y; y++)
				bit ^= data[head.y*x + y];
		if (bit != read_data[n])
		{
			if (warring != -1)
				stat_ok = false;
			else
				warring = n;
		}

	}
	if (!stat_ok)
		return FAILD;
	if (warring == -1)
		return OK;
	int y_warring = -1;
#pragma omp parallel for
	for (int n = 0; n < head.y; n++)
	{
		char bit = 0;
		for (int y = 0; y < head.y; y++)
			for (int x = 0; x < head.x; x++)
				bit ^= data[head.x*y + x];
		if (bit != read_data[head.x + n])
		{
			if (y_warring == -1)
				y_warring = n;
			else
				y_warring = -2;
		}
	}
	if (y_warring == -1 || y_warring == -2)
		return FAILD;
	int64_t off = warring*head.x + y_warring;
	char fix_data = 0;
	bool fix = false;
#undef min
#undef max
#pragma omp parallel for
	for (int b = numeric_limits<char>::min(); b < numeric_limits<char>::max(); b++)
	{
		char cx = 0;
		char cy = 0;
		for (int y = 0; y < head.y; y++)
			cx ^= data[head.y*warring + y];
		for (int x = 0; x < head.x; x++)
			cy ^= data[head.x*y_warring + x];
		if (cx == read_data[warring] && cy == read_data[y_warring + head.x])
		{
			fix = true;
			fix_data = b;
		}
	}
	if (!fix)
		return FAILD;
	data[off] = fix_data;
	return Fixed;
}

API STAT VerifyRecoveryData(const char * rec, int64_t len)
{
	REC_HEAD head;
	memcpy(&head, rec, sizeof(REC_HEAD));
	int64_t splite = len / 2;
	if (getsumV2(rec +sizeof(REC_HEAD) , get_block_len(len) - sizeof(REC_HEAD)) != head.checksum)
		return FAILD;
	return OK;
}
