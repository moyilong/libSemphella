#include "main.h"
#include "files.h"
#include <fstream>
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include "crypt.h"
#include "string.h"


bool f2verbos = false;
#define f2debug if (f2verbos) debug

file::~file()
{
	fflush(fp);
	close();
}

void file::close()
{
	if (!opend)
	{
		f2debug << ioname << " is already closed!" << endl;
		return;
	}
	fflush(fp);
	opend = false;
	return;
}

void file::read(char *buff, uint64_t len)
{
	check();
	fread(buff, sizeof(char), len / sizeof(char), fp);
	f2debug << "Read Checksum:" << getsumV2(buff, len)<<endl;
}

void file::write(char *buff, uint64_t len)
{
	fflush(fp);
	check();
	fwrite(buff, sizeof(char), len / sizeof(char), fp);
	f2debug << "Write Checksum:" << getsumV2(buff, len) << endl;
}

bool file::open(string filename, string mode)
{
	if (opend)
		return false;
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
	fseek(fp, off, 0);
	f2debug << "Seek is redirect to " << ftell(fp) << endl;
}

void file::check()
{
	if (opend && fp == NULL)
	{
		f2debug << "Error Report!" << endl;
		f2debug << "Status is open bug the status is ptr is null" << endl;
		abort();
	}
}