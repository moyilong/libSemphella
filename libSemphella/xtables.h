#pragma once
#include "main.h"
#include "libSemphella.h"

template<class DATA,class INDEX>class xtables
{
public:
	struct IDX {
		DATA data;
		INDEX idx;
	};
	xtables();
	~xtables();
	void add(DATA dat, INDEX idx);
	DATA search(INDEX idx);
	inline uint64_t size()
	{
		return data.size();
	}
	inline void clear()
	{
		data.clear();
	}
private:
	vector<IDX>data;
	void memtest(void *a, void *b, uint64_t len);
};

template<class DATA, class INDEX>
inline xtables<DATA, INDEX>::xtables()
{
}

template<class DATA, class INDEX>
inline xtables<DATA, INDEX>::~xtables()
{
	data.clear();
}

template<class DATA, class INDEX>
inline void xtables<DATA, INDEX>::add(DATA dat, INDEX idx)
{
	IDX temp;
	temp.data = dat;
	temp.idx = idx;
	data.push_back(temp);
}

template<class DATA, class INDEX>
inline DATA xtables<DATA, INDEX>::search(INDEX idx)
{
	int id = -1;
#pragma omp parallel for
	for (int64_t n = 0;n < data.size();n++)
		if (sizeof(idx) == sizeof(data.at(n).idx))
			if (memtest(data.at(n).idx, idx,sizeof(idx) ))
				id = n;
	if (id == -1)
		return NULL;
	return data.at(id).data;
}

template<class DATA, class INDEX>
inline void xtables<DATA, INDEX>::memtest(void * a, void * b, uint64_t len)
{
	for (int n = 0;n < len;n++)
		if ((char)a[n] != (char)b[n])
			return false;
	return true;
}
