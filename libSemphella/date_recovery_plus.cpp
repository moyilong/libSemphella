#include "main.h"
#include "string.h"
#include "debug.h"
#include "date_recovery_plus.h"
#include <math.h>

#ifdef __ANDROID__
#define sqrtl sqrt
#endif

void LenToXY(uint64_t len, uint64_t &x, uint64_t &y)
{
	uint64_t ret_x = sqrtl(len);
	uint64_t ret_y = len / ret_x;
	if (ret_x*ret_y < len)
		ret_y += 1;
	x = ret_x;
	y = ret_y;
	debug << "Return:" << x <<","<<y<< endl;
}

API uint64_t GetLength(uint64_t len)
{
	uint64_t x, y;
	LenToXY(len, x, y);
	return x + y;
}

inline char link_2d_3d(const char *data, uint64_t len, uint64_t px, uint64_t py, uint64_t rx, uint64_t ry, bool xasix, uint64_t replace_point = -1, char tar = -1)
{
	uint64_t ref_2d=px*ry+rx;
	if (ref_2d > len)
	{

		return 0xFF;
	}
	if (replace_point != -1)
		if (ref_2d == replace_point)
			return tar;
	return data[ref_2d];
}

API void CreateRefPoint(const char *data, uint64_t len, uint64_t &wback, char *back)
{
	uint64_t x, y;
	LenToXY(len, x, y);
	
	//back = (char*)malloc(x + y);
	memset(back, 0, x + y);
	for (uint64_t rx = 0; rx < x; rx++)
		for (uint64_t ry = 0; ry < y; ry++)
		{
			back[rx] ^= link_2d_3d(data, len, x, y, rx, ry, true);
		}
	debug << "Once wave compltet!" << endl;
	for (uint64_t ry = 0; ry < y; ry++)
	{
		for (uint64_t rx = 0; rx < x; rx++)
		{
			back[x + ry] ^= link_2d_3d(data, len, x, y, rx, ry, false);
		}
	}
	wback = x + y;
}
#include <limits>
API int RefPointCorrect(char *data, uint64_t len, const char *code)
{
#undef min
#undef max
	uint64_t px, py;
	LenToXY(len, px, py);
	debug << "RefPX_PY=" << px << "," << py << endl;
	char *new_code = (char*)malloc(px + py);
	uint64_t klen;
	CreateRefPoint(data, len, klen, new_code);
	int as = 0;
	uint64_t x = -1;
	uint64_t y = -1;
	if (px==py)
	for (int n=0;n<px;n++)
		if (code[n] == code[n + px])
		{
			debug << "No Different @" << n << endl;
		}
	for (uint64_t n = 0; n < px; n++)
		if (code[n] != new_code[n])
		{
			as++;
			x = n;
			debug << "X Find Different at:" << n << endl;
		}
	for (uint64_t n = 0; n < py; n++)
		if (code[n+px] != new_code[n+px])
		{
			as++;
			y = n;
			debug << "Y Find Different at:" << n << endl;
		}
	free(new_code);
	debug << "Value as=" << as << endl;
	if (as != 2)
	{
		debug << "not a normally status!" << endl;
		if (as == 0)
			return 0;
		if (as == 1)
			return ERR_CODE_FAILD;
		if (as > 2)
			return ERR_CORRECT_ER;
	}
	uint64_t position = px*y + x;
	debug << "Find Problem in " << position<< "(" << x << "," << y << ")" << endl;
	vector<char>tx, ty;
#pragma omp parallel for
	for (char b = numeric_limits<char>::min(); b < numeric_limits<char>::max(); b++)
	{
		char test = 0;
		for (uint64_t rx = 0; rx < px; rx++)
		{
			test ^= link_2d_3d(data, len, px, py, rx, y, false, position, b);
		}
		if (test == code[px + y])
			ty.push_back(b);


		test = 0;
		for (uint64_t ry = 0; ry < py; ry++)
		{
			test ^= link_2d_3d(data, len, px, py, x, ry, true, position, b);
		}
		if (test == code[x])
			tx.push_back(b);
	}
	debug << "All Find X=" << tx.size() << " Y=" << ty.size() << endl;
	vector<char> fin;
	for (int n = 0; n < tx.size(); n++)
		for (int m = 0; m < ty.size(); m++)
			if (tx.at(n) == ty.at(m))
				fin.push_back(tx.at(n));
	if (fin.size() != 1)
	{
		debug << "There to many value to find!" << endl;
		debug << "All Find Value:" << fin.size() << endl;
		return ERR_CODE_FAILD;
	}
	data[px*y + x] = fin.at(0);
	return 0;
}

API bool TrustyTestPerFrame(uint64_t seek, uint64_t length)
{
	char *buff[3];
	for (int n = 0; n < 3; n++)
	{
		buff[n] = (char*)malloc(length);
		memset(buff[n], 0, length);
	}
	char dat;
	for (int n = 0; n < length; n++)
	{
		dat ^= time(0)*seek + rand();
		buff[0][n] = dat;
		buff[1][n] = dat;
		buff[2][n] = dat;
	}
	int64_t _ref_point = sin(dat)*length;
	uint64_t ref_point;
	if (_ref_point < 0)
		ref_point = -_ref_point;
	else
		ref_point = _ref_point;
	buff[1][ref_point] = ~(buff[1][ref_point] ^ 7) ^8;
	buff[2][ref_point] = buff[1][ref_point];
	debug << "Set Problem in " << ref_point <<" "<< (int)buff[0][ref_point] << "=>" << (int)buff[1][ref_point] << endl;
	char *ecode = (char*)malloc(GetLength(length));
	char *ecode2 = (char*)malloc(GetLength(length));
	uint64_t len;
	CreateRefPoint(buff[0], length, len, ecode);
	CreateRefPoint(buff[0], length, len, ecode2);
	display_dump(ecode,len);
	for (int n = 0; n < len; n++)
	{
		bool stat = true;
		if (ecode[n] != ecode2[n])
		{
			cout << "Code Unmatch:" << n << endl;
			stat = false;

		}
		if (!stat)
		{
			free(buff[0]); 
				free(buff[1]); 
				free(buff[2]); 
				free(ecode); 
				free(ecode2); 
				return false;
		}
	}
	int ret = RefPointCorrect(buff[1], length, ecode);
	if (ret != 0)
	{
		free(buff[0]);
		free(buff[1]);
		free(buff[2]);
		free(ecode);
		free(ecode2);
		return false;
	}
	bool stat = true;
	for (uint64_t n = 0; n < length; n++)
	{
		if (buff[0][n] != buff[1][n])
		{
			stat = false;
			cout << "Correct Point Find At:" << n;
			if (n == ref_point)
				cout << " UNFIX!" << endl;
			else
				cout << " REWRITE!" << endl;
		}
	}
	free(buff[0]);
	free(buff[1]);
	free(buff[2]);
	free(ecode);
	free(ecode2);
	return true;


}