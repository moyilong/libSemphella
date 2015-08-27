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