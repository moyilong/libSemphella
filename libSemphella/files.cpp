#include "main.h"
#include "files.h"
#include <fstream>
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include "crypt.h"
#include "string.h"
#define f2debug  debug

file::~file()
{
}

void file::close()
{
}

void file::read(char *buff, uint64_t len)
{
	check();
	fread(buff, sizeof(char), len / sizeof(char), fp);
}

void file::write(char *buff, uint64_t len)
{
	check();
	fwrite(buff, sizeof(char), len / sizeof(char), fp);
}

bool file::open(string filename, string mode)
{
	if (opend)
		return false;
	debug << "OpenFile:" << filename << " by " << mode << endl;
	fp = fopen(filename.data(), mode.data());
	if (fp == NULL)
		return false;
	opend = true;
	uint64_t te = ftell(fp);
	fseek(fp, 0, SEEK_END);
	te = ftell(fp) - te;
	len = te;
	fseek(fp, 0, 0);
	f2debug << "Setting File Len:" << len << endl;
	debug << "File:" << filename << " was been opend!" << endl;
	return true;
}

uint64_t file::tellp()
{
	check();
	return ftell(fp);
}

uint64_t file::tell_len()
{
	check();
	return len;
}

void file::seekp(uint64_t off)
{
	check();
	if (off<0 && off>len)
		return;
	f2debug << "Redirect " << tellp() << " => " << off << endl;
	fseek(fp, off, 0);
}

bool file::is_eof()
{
	check();
	return feof(fp);
}

void file::get_steps(uint64_t bs, uint64_t & mbs, uint64_t & fix)
{
	uint64_t len = tell_len();
	mbs = len / bs;
	fix = len - (mbs*bs);
}

uint64_t file::getsum()
{
	char buff[MAX_BUFF_SIZE];
	uint64_t bs;
	uint64_t fix;
	get_steps(MAX_BUFF_SIZE, bs, fix);
	uint64_t sum = 0;
	for (int n = 0; n < bs; n++)
	{
		seekp(n*MAX_BUFF_SIZE);
		memset(buff, 0, MAX_BUFF_SIZE);
		read(buff, MAX_BUFF_SIZE);
		sum += getsumV2(buff, MAX_BUFF_SIZE);
	}
	if (fix > 0)
	{
		seekp(len - fix);
		memset(buff, 0, MAX_BUFF_SIZE);
		read(buff, fix);
		sum += getsumV2(buff, fix);
	}
	return sum;
}

void file::snapshot(uint64_t addr)
{
	if (snapshot_addr != -1)
	{
		return;
	}
	snapshot_addr = tellp();
	seekp(addr);
}

void file::desnapshot()
{
	if (snapshot_addr == -1)
	{
		return;
	}
	seekp(snapshot_addr);
	snapshot_addr = -1;
}

string file::getline(uint64_t perfect_max)
{
	char *buff = (char*)malloc(perfect_max);
	fgets(buff, perfect_max, fp);
	string ret = buff;
	free(buff);
	return ret;
}

void file::check()
{
	if (!opend || fp == NULL)
	{
		f2debug << "Error Report!" << endl;
		f2debug << "Status is open bug the status is ptr is null" << endl;
		KERNEL.abort();
	}
}

API void fs_verbos(bool stat)
{
	KERNEL.SetDebugStat(stat);
}

void file::write(char *buff, uint64_t len, uint64_t off)
{
	seekp(off);
	write(buff, len);
}

void file::flush()
{
	check();
	fflush(fp);
}

void file::read(char *buff, uint64_t len, uint64_t off)
{
	check();
	seekp(off);
	read(buff, len);
}