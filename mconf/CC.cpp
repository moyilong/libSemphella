#include "stdafx.h"
#include "loaddef.h"

void WriteString(string str, ofstream &out)
{
	int len = str.size();
	out.write((char*)&len, sizeof(int));
	out.write(str.data(), str.size());
}

string  ReadString(ifstream &in)
{
	int len = 0;
	in.read((char*)&len, sizeof(int));
	char *buff = new char[len];
	in.read(buff, len);
	string ret = buff;
	free(buff);
	return ret;
}

void WriteBlock(BLOCK_INFO blk,ofstream &out)
{
	WriteString(blk.uci_info, out);
	WriteString(blk.prompt, out);
	WriteString(blk.type, out);
	WriteString(blk.depends, out);
	WriteString(blk.name, out);
	WriteString(blk.section, out);
	WriteString(blk.display, out);
	WriteString(blk.father, out);
	WriteString(blk.default_val, out);	
}

BLOCK_INFO ReadBlock(ifstream &in)
{
	BLOCK_INFO blk;
	blk.uci_info = ReadString(in);
	blk.prompt = ReadString(in);
	blk.type = ReadString(in);
	blk.depends = ReadString(in);
	blk.name = ReadString(in);
	blk.section = ReadString(in);
	blk.display = ReadString(in);
	blk.father = ReadString(in);
	blk.default_val = ReadString(in);
	return blk;
}