// EFU.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "format.h"
#include <libSemphella/main.h>
#include <libSemphella/libSemphella.h>
#include <libSemphella/string.h>
string in;
string out;
FF_HEAD info;

int main(int argc,char *argv[])
{
	for (int n = 1; n < argc; n++)
	if (argv[n][0]=='-')
		switch (argv[n][1])
		{
		case 'b':
			info.build_ver = atol(argv[n] + 2);
			break;
		case 'n':
			strcpy(info.IMG_NAME, argv[n] + 2);
			break;


		}

	if (!strcmp(argv[1], "img2ffu"))
	{
		ifstream inf;
		ofstream off;

		uint64_t flen = 0;
		inf.seekg(ios::beg);
		flen = inf.tellg();
		inf.seekg(ios::end);
		uint64_t xlen = inf.tellg();
		info.file_len = xlen - flen;
		off.write((char*)&info, sizeof(FF_HEAD));
		while (!inf.eof())
		{
			char buff[MAX_BUFF_SIZE];
			memset(buff, 0, sizeof(MAX_BUFF_SIZE));
			inf.read(buff, MAX_BUFF_SIZE);
			off.write(buff, MAX_BUFF_SIZE);
		}
	}
	else {

	}
}

