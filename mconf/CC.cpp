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

void WriteBlock(BLOCK_INFO blk, ofstream &out)
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

struct MENU_WRITE {
	long long sub_menu_size;
	long long data_size;
};

void WriteMenu(MENU menu, ofstream &out)
{
	MENU_WRITE target;
	target.sub_menu_size = menu.mdata.size();
	target.data_size = menu.data.size();
	out.write((char*)&target, sizeof(MENU_WRITE));
	WriteString(menu.section, out);
	WriteString(menu.display_name, out);
	for (int n = 0; n < menu.data.size(); n++)
		WriteBlock(menu.data.at(n), out);
	for (int n = 0; n < menu.mdata.size(); n++)
		WriteMenu(menu.mdata.at(n), out);
}

MENU ReadMenu(ifstream &in)
{
	MENU temp;
	MENU_WRITE target;
	in.read((char*)&target, sizeof(MENU_WRITE));
	temp.section = ReadString(in);
	temp.display_name = ReadString(in);
	for (int n = 0; n < target.data_size; n++)
		temp.data.push_back(ReadBlock(in));
	for (int n = 0; n < target.sub_menu_size; n++)
		temp.mdata.push_back(ReadMenu(in));
	return temp;
}

struct MCONF_CACHE {
	char api = MAPI_LEVEL;
};

void Write(string filename)
{
	MCONF_CACHE head;
	ofstream read;
	read.open(filename.data());
	if (!read.is_open())
	{
		DEBUG << "Open File Faild!" << endl;
		exit(-1);
	}
	head.api = MAPI_LEVEL;
	read.write((char*)&head, sizeof(MCONF_CACHE));
	WriteMenu(main_menu, read);
	read.close();
}

void Read(string filename)
{
	ifstream read;
	read.open(filename.data());
	if (!read.is_open())
	{
		DEBUG << "Open File Faild!" << endl;
		exit(-1);
	}
	MCONF_CACHE head;
	read.read((char*)&head, sizeof(MCONF_CACHE));
	if (head.api != MAPI_LEVEL)
	{
		DEBUG << "Reading Cache's API is mismatch!" << endl;
	}
	main_menu = ReadMenu(read);
	read.close();
}