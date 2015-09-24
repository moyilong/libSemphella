#include <vector>

template<class T> inline T max(T a, T b)
{
	if (a > b)
		return a;
	return b;
}

template<class T> inline T min(T a, T b)
{
	if (a > b)
		return b;
	return a;
}

template<class T>inline T dZero(T n)
{
	if (n == 0)
		return 1;
	return n;
}


#define INT_LEN	4096