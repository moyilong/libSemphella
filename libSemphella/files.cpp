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
	close();
}

void file::close()
{
	if (!opend)
	{
		f2debug << ioname << " is already closed!" << endl;
		return;
	}
	opend = false;
	return;
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
	if (opend)
	{
		uint64_t te = ftell(fp);
		fseek(fp, 0, SEEK_END);
		te = ftell(fp) - te;
		len = te;
		fseek(fp, 0, 0);
		f2debug << "Setting File Len:" << len << endl;
	}
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
	return len;
}

void file::seekp(uint64_t off)
{
	check();
	f2debug << "Redirect " << tellp() << " => " << off << endl;
	fseek(fp, off, 0);
}

bool file::is_eof()
{
	return feof(fp);
}

void file::check()
{
	if (opend && fp == NULL)
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