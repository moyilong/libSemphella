#pragma once
#include "libSemphella.h"

class CAPI file {
public:
	~file();
	void seekp(uint64_t off);
	uint64_t tellp();
	uint64_t tell_len();
	bool open(string filename, string mode);
	void close();
	void read(char *buff, uint64_t len);
	void write(char *buff, uint64_t len);
	inline bool is_open()
	{
		return opend;
	}
	inline void flush()
	{
		fflush(fp);
	}
	template<class type> void read(type *buff, uint64_t block_size);
	template<class type> void write(type *buff, uint64_t block_size);

	bool is_eof();
private:
	FILE *fp;
	string ioname;
	string mode;
	bool opend = false;
	uint64_t len;
protected:
	void check();
};

template<class type>
inline void file::read(type * buff, uint64_t block_size)
{
	fread(buff, sizeof(type), block_size, fp);
}

template<class type>
inline void file::write(type * buff, uint64_t block_size)
{
	fwrite(buff, sizeof(type), block_size, fp);
}

API void fs_verbos(bool stat);