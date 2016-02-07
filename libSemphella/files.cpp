#include "main.h"
#include "files.h"
#include <fstream>
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include "crypt.h"
#include "string.h"
file::~file()
{
	close();
}

void file::close()
{
	if (!opend)
	{
		debug << ioname << " is already closed!" << endl;
		return;
	}
	fflush(fp);
	opend = false;
	return;
}

void file::read(char *buff, uint64_t len)
{
	check();
	debug << "Read form Address:" << ftell(fp) << endl;
	uint64_t rlen = len / sizeof(char);
	uint64_t rblk = sizeof(char);
	uint64_t rsize = fread(buff, sizeof(char), len / sizeof(char), fp);
	if (rsize != rlen)
	{
		debug << "Real read size:" << rsize << " need read size:" << rlen << endl;
		KERNEL.error("Read Size Mismatch!");
	}
	debug << "Read Checksum:" << getsumV2(buff, len)<<endl;
}

void file::write(char *buff, uint64_t len)
{
	fflush(fp);
	check();
	debug << "Write Checksum:" << getsumV2(buff, len) << endl;
	if (fwrite(buff, sizeof(char), len / sizeof(char), fp) != len)
		KERNEL.error("Write Size Mismatch!");
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
		debug << "Setting File Len:" << len << endl;
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
	debug << "Seek is redirect to " << ftell(fp) << endl;
}

void file::check()
{
	if (opend && fp == NULL)
	{
		debug << "Error Report!" << endl;
		debug << "Status is open bug the status is ptr is null" << endl;
		abort();
	}
}