#include "libSemphella.h"
#include "math.h"
#include "crypt.h"
#include "debug.h"
API XPOINT VectorPoint2D(XPOINT orig, MTYPE _dgst, MTYPE len)
{
	MTYPE a;
	MTYPE b;
	MTYPE dgst = _dgst;
	a = sin(dgst) / len;
	b = sqrt(len*len - a*a);
	XPOINT ret = orig;
	ret.x += a;
	ret.y += b;
	return ret;
}

API XPOINT ShadowX(XPOINT point, CST_TYPE type)
{
	switch (type)
	{
	case X:
		return XPOINT(0, point.y, point.z);
		break;
	case Y:
		return XPOINT(point.x, 0, point.z);
		break;
	case Z:
		return XPOINT(point.x, point.y, 0);
		break;
	}
	return point;
}

API XPOINT LineGetPoint(MTYPE x, const LINE line)
{
	return XPOINT(x, line.k*x + line.b);
}

API XPOINT PointMove(XPOINT orig, XPOINT diff)
{
	return XPOINT(orig.x + diff.x, orig.y + diff.y, orig.z + diff.z);
}

API	MTYPE PointToPoint(XPOINT a, XPOINT b)
{
	MTYPE dx = abs(a.x - b.x);
	MTYPE dy = abs(a.y - b.y);
	MTYPE dz = abs(a.z - b.z);
	return sqrt(dx*dx + dy*dy + dz*dz);
}

API MTYPE LINE_2D_K(XPOINT a, XPOINT b)
{
	if (a.y - b.y == 0 || a.x - b.x == 0)
		return 0;
	return abs(a.y - b.y) / abs(a.x - b.x);
}

API MTYPE LIM_RAND(MTYPE min, MTYPE max, MTYPE deep)
{
	MTYPE ret = sin(deep + rand())*(max - min) + min;
	if (ret<min || ret > max)
		return LIM_RAND(min, max, deep + 1);
	return ret;
}

API uint64_t LIM_RAND_ULD(uint64_t min, uint64_t max, uint64_t deep)
{
	uint64_t ret = sin(deep + rand())*(max - min) + min;
	if (ret<min || ret > max)
		return LIM_RAND_ULD(min, max, deep + 1);
	return ret;
}
#include <limits>
API bool is_prime(uint64_t value)
{
	bool stat = true;
#undef min
#undef max
#pragma omp parallel for
	for (int64_t n = numeric_limits<int64_t>::min(); n < numeric_limits<int64_t>::min() + value; n++)
	{
		if (!stat)
			continue;
		uint64_t val = n - numeric_limits<int64_t>::min();
		if (n == 0 || n == 1 || n == value)
			continue;
		if (value%val == 0)
			stat = false;
	}
	return stat;
}
#define PREDEF "moyilong_predef"
uint64_t random_seek=getsumV2(PREDEF,strlen(PREDEF));
API void random(char * buff, int64_t len,uint64_t loop_size)
{
	char cbuff[sizeof(uint64_t)];
	memcpy(cbuff, &random_seek,sizeof(uint64_t));
#pragma omp parallel for
	for (int64_t n = 0; n < len; n++)
	{
		int id =  sin((n + 23) + sizeof(uint64_t) + random_seek);
		if (id < 0)
			id = -id;
		buff[n] = cbuff[id];
	}
	random_seek++;
	random_seek += getsumV2(buff, len);
	for (int n = 0; n < loop_size; n++)
		random(buff, len, 0);
}
API void random_test()
{
	uint64_t val = 0;
	vector<uint64_t>valx;
	while (true)
	{
		cout << random() << endl;
	}
}
API vector<LIMIT_LINE> ShortX(vector<LIMIT_LINE> data)
{
	vector<LIMIT_LINE> ret;
	vector<uint64_t> find;
	for (int64_t n = 0; n < data.size(); n++)
	{
		if (n == 0)
		{
			ret.push_back(data.at(0));
			continue;
		}
		int64_t fid = -1;
		bool swap = false;
#pragma omp parallel for
		for (int64_t n = 0; n < data.size(); n++)
		{
			if (fid != -1)
				continue;
			if (n != 0)
			{
				bool check = true;
				for (uint64_t x = 0; x < find.size(); x++)
					if (check)
						if (find.at(x) == n)
							check = false;
				if (!check)
					continue;
			}
			if (data.at(n).a.equal(ret.at(ret.size() - 1).b))
			{
				fid = n;
				continue;
			}
			if (data.at(n).b.equal(ret.at(ret.size() - 1).b))
			{
				fid = n;
				swap = true;
				continue;;
			}
		}
		if (fid == -1)
		{
			cout << "Error Unexcepted Status! No Next Line Found!" << endl;
			exit(-1);
		}
		if (fid == 0)
		{
			continue;
		}
		else {
			if (swap)
			{
				ret.push_back(LIMIT_LINE(data.at(fid).b, data.at(fid).a));
			}
			else {
				ret.push_back(data.at(fid));
			}
		}
	}
	return ret;
}
#include "debug.h"
API void ShortXTest(uint64_t test_numbers,string outformal)
{
	debug << "Perparing Memory..." << endl;
	vector<LIMIT_LINE> memm(test_numbers);
	memm.push_back(LIMIT_LINE(XPOINT(), XPOINT()));
	for (uint64_t n = 0; n < test_numbers; n++)
	{
		int ra = 0;
		random();
		XPOINT a = XPOINT();
		LIMIT_LINE line;

		if (ra % 2 == 0)
		{
			line.a = a;
			line.b = memm.at(memm.size() - 1).b;
		}
		else {
			line.b = a;
			line.a = memm.at(memm.size() - 1).b;
		}
		debug << "GeneratedA," << line.a.x << "," << line.a.y << "," << line.a.z << endl;
		debug << "GeneratedB," << line.b.x << "," << line.b.y << "," << line.b.z << endl;
	}
	memm.push_back(LIMIT_LINE(memm.at(0).a, (memm.at(memm.size() - 1).b)));
	debug << "Complete!";
	vector<LIMIT_LINE> get = ShortX(memm);
	debug << "Geted!" << endl;
	for (int64_t n = 0; n < get.size(); n++)
	{
		printf(outformal.data(), get.at(n).a.x, get.at(n).a.y, get.at(n).a.z, get.at(n).b.x, get.at(n).b.y, get.at(n).b.z);
	}
}


uint64_t block_math::Addr2Block(uint64_t addr, uint64_t & blk, uint64_t & off,bool with_head)
{
	if (with_head)
		addr -= begin_offset;
	return addr;
}

XPOINT::XPOINT()
{
	x = random();
	y = random();
	z = random();
}
XPOINT::XPOINT(MTYPE _x, MTYPE _y)
{
	x = _x;
	y = _y;
}
XPOINT::XPOINT(MTYPE _x, MTYPE _y, MTYPE _z)
{
	x = _x;
	y = _y;
	z = _z;
}
bool XPOINT::equal(const XPOINT b)
{
	if (b.x == x&&b.y == y&&b.z == z)
		return true;
	return false;
}

uint64_t XPOINT::hash()
{
	MTYPE buff[3] = { x,y,z };
	return getsumV2((char*)buff, sizeof(buff));
}
