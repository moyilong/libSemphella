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

template<class C>inline int get_type_len(C value)
{
	C temp;
	memset(&temp, 0xFF, sizeof(C));
	char buff[MAX_BUFF_SIZE] = { 0x00 };
	sprintf(buff, "%d", temp);
	return strlen(buff);
}

#define INT_LEN	4096

class API int_c {
public:
	int_c();
	~int_c();
	int_c operator +(int_c a);
	int_c operator -(int_c a);
	int_c operator *(int_c a);
	int_c operator /(int_c a);
	int_c operator ^(int_c b);
	int_c operator |(int_c b);
	int_c operator &(int_c b);
	
	operator int();
	operator float();
	operator long long();
	operator long double();
	operator string();

	bool operator ==(int_c b);

	char *get_int();
	char *get_float();

	void random();
private:
	char int_t[INT_LEN];
	char float_t[INT_LEN];
protected:
	void int_check();
	void float_check();
};

