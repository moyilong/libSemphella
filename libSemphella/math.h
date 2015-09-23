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


#ifndef __LINUX__
template<class C>inline int get_type_len(C value)
{
	C temp;
	memset(&temp, 0xFF, sizeof(C));
	char buff[MAX_BUFF_SIZE] = { 0x00 };
	sprintf(buff, "%d", temp);
	return strlen(buff);
}

#define INT_LEN	4096