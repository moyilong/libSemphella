
#include "libSemphella.h"
#include "math.h"

int_c::int_c()
{
	sZero(int_t, 0, INT_LEN);
	sZero(float_t, 0, INT_LEN);
}

int_c::~int_c()
{
}

inline char * int_c::get_int()
{
	return int_t;
}

inline char * int_c::get_float()
{
	return float_t;
}

inline void int_c::int_check()
{
	for (int n = 0; n < INT_LEN; n++)
	{
		if (float_t[n] > 10)
		{
			if (n != INT_LEN - 1)
			{
				char n = 0;
				n=int_t[n] % 10;
				int_t[n] -= n*10;
				float_t[n + 1] += n;
			}
		}
	}
}

inline void int_c::float_check()
{
	for (int n = 0; n < INT_LEN; n++)
	{
		if (float_t[n] > 10)
		{
			if (n != INT_LEN - 1)
			{
				char n = 0;
				n=float_t[n] % 10;
				float_t[n] -= n*10;
				float_t[n + 1] += n;
			}
		}
	}
	if (float_t[0] > 10)
	{
		float_t[0] -= 10;
		int_t[0] += 1;
	}
	int_check();
}



int_c int_c::operator+(int_c a)
{
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
	{
		int_t[n] += a.get_int()[n];
		float_t[n] += a.get_float()[n];
	}
	float_check();
	return *this;
}

int_c int_c::operator-(int_c a)
{
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
	{
		int_t[n] -= a.get_int()[n];
		float_t[n] -= a.get_float ()[n];
	}
	float_check();
	return *this;
}

int_c int_c::operator*(int_c a)
{
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
	{
		int_t[n] *= a.get_int()[n];
		float_t[n] *= a.get_float()[n];
	}
	float_check();
	return *this;
}

int_c int_c::operator/(int_c a)
{
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
	{
		int_t[n] /= a.get_int()[n];
		float_t[n] /= a.get_float()[n];
	}
	float_check();
	return *this;
}

int_c int_c::operator^(int_c a)
{
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
	{
		int_t[n] ^= a.get_int()[n];
		float_t[n] ^= a.get_float()[n];
	}
	float_check();
	return *this;
}

int_c int_c::operator|(int_c a)
{
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
	{
		int_t[n] |= a.get_int()[n];
		float_t[n] |= a.get_float()[n];
	}
	float_check();
	return *this;
}

int_c int_c::operator&(int_c a)
{
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
	{
		int_t[n] &= a.get_int()[n];
		float_t[n] &= a.get_float()[n];
	}
	float_check();
	return *this;
}

int_c::operator int()
{
	int ret = 0;
	for (int n = 0; n < get_type_len((int)0); n++)
		ret += int_t[n] * pow(10, n);
	return ret;
}

int_c::operator long long()
{
	long long ret = 0;
	for (int n = 0; n < get_type_len((long long)0); n++)
		ret += int_t[n] * pow(10, n);
	return ret;
}


int_c::operator float()
{
	float ret=0;
	for (int n = 0; n < get_type_len((float)0.00); n++)
	{
		ret += int_t[n] * pow(10, n);
		ret += float_t[n] / pow(10, n);
	}
	return ret;
}

int_c::operator long double()
{
	long double ret = 0;
	for (int n = 0; n < get_type_len((long double)0.00); n++)
	{
		ret += int_t[n] * pow(10, n);
		ret += float_t[n] / pow(10, n);
	}
	return ret;
}


int_c::operator string()
{
	string int_s,int_f;
	for (int n = 0; n < INT_LEN;n++)
	{
		string int_sint;
		char buff[MAX_BUFF_SIZE] = { 0x00 };
		sprintf(buff, "&d", int_t[n]);
		int_sint = buff;
		int_s = int_sint+int_s;
		sZero(buff,0,MAX_BUFF_SIZE);
		sprintf(buff, "%d", float_t[n]);
		int_sint = buff;
		int_f += int_sint;
	}
	string ret = int_s;
	ret += ".";
	ret += int_t;
	return ret;
}

bool int_c::operator==(int_c b)
{
	bool ret = true;
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
		if (b.get_float()[n] != float_t[n] || b.get_int()[n] != int_t[n])
			ret = false;
	return ret;
}

inline char crandom(char min, char max)
{
	char ret = rand();
	while (ret >= min&&ret <= max)
		ret = rand();
	return ret;
}

void int_c::random()
{
#pragma omp parallel for
	for (int n = 0; n < INT_LEN; n++)
	{
		int_t[n] = crandom(0, 9);
		float_t[n] = crandom(0, 9);
	}
}