#pragma once
#include "main.h"
#include "libSemphella.h"
#include "crypt.h"
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
	inline uint64_t get_hash()
	{
		uint64_t *mem = (uint64_t *)malloc(sizeof(uint64_t)*data.size());
#pragma omp parallel for
		for (int64_t n = 0;n < data.size();n++)
			mem[n] = getsumV2((char*)data.at(n).data, sizeof(data));
		uint64_t ret = 0;
		for (int64_t n = 0;n < data.size();n++)
			ret += mem[n];
		free(mem);
		return ret << 8;
	}
	inline DATA at(uint64_t size)
	{
		return data.at(size).data;
	}
	inline IDX idx(uint64_t size)
	{
		return data.at(size);
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
	char *ptr_a=a;
	char *ptr_b=b;
	for (int n = 0;n < len;n++)
		if (ptr_a[n]!=ptr_b[n])
			return false;
	return true;
}
