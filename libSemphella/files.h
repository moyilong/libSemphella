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
private:
	FILE *fp;
	string ioname;
	string mode;
	bool opend = false;
	uint64_t len;
protected:
	void check();
};