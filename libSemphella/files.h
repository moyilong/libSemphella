#pragma once
#include "libSemphella.h"
#include "debug.h"

typedef void(*f_process)(const string val);

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
	inline bool is_open() const
	{
		return opend;
	}
	void flush();
	template<class type> void read(type *buff, uint64_t block_size);
	template<class type> void write(type *buff, uint64_t block_size);
	void read(char *buff, uint64_t len, uint64_t off);
	void write(char *buff, uint64_t len, uint64_t off);
	bool is_eof();
	void get_steps(uint64_t bs, uint64_t &mbs, uint64_t &fix);
	uint64_t getsum();
	void snapshot(uint64_t addr);
	void desnapshot();
	string getline(uint64_t perfect_max = 8192);
	void write(string str);
	string read();
	void reopen();
	void for_each_line(f_process api);
protected:
	void check();
	uint64_t snapshot_addr = -1;
	FILE *fp = NULL;
	string ioname;
	string mode;
	bool opend = false;
	uint64_t len;
	bool over_head_check(uint64_t len);
};

template<class type>
inline void file::read(type * buff, uint64_t block_size)
{
	debug << "ReadFile " << ioname << "@" << tellp() << "+" << sizeof(type)*block_size << endl;
	fread(buff, sizeof(type), block_size, fp);
}

template<class type>
inline void file::write(type * buff, uint64_t block_size)
{
	debug << "WriteFile " << ioname << "@" << tellp() << "+" << sizeof(type)*block_size << endl;
	fwrite(buff, sizeof(type), block_size, fp);
}

