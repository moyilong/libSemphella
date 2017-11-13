// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/libSemphella.h>
#include <libSemphella/argment.h>
#include <libSemphella/string.h>
#include <libERT/libERT.h>
#include <libERT/ext_io.h>
#include <libERT/libERT.h>
estring input, output, password,extfile;
bool decrypt = false;
int algr = DEFAULT_ALG_ID;
int bs = 16*KB;
void proc(string _name, string value)
{
	estring name = estring(_name).ToLower();
	switch (name.at(0))
	{
	case 'i':
		input = value;
		break;
	case 'o':
		output = value;
		break;
	case 'p':
		password = value;
		break;
	case 'P':
		PerformanceTest();
		exit(0);
		break;
	case 'd':
		decrypt = true;
		break;
	case 'a':
		algr = atoi(value.data());
		break;
	case 'e':
		extfile = value;
		break;
	case 'b':
		bs = atoi(value.data());
		break;
	}
}
void Help()
{
	cout << "============Help of CryptUtils==============" << endl;
	cout << "\t-i\tInput File" << endl;
	cout << "\t-o\tOutput File" << endl;
	cout << "\t-p\tPassword" << endl;
	cout << "\t-P\tPerformance Test Mode" << endl;
	cout << "\t-d\tDecrypt Mode" << endl;
	cout << "\t-a\tSet Algorthim ID" << endl;
	cout << "\t-e\tSet Extentision File" << endl;
	cout << "\t-b\tSet Block Length" << endl;
}
void Progress(double p)
{
	ShowProcessBar(p*100, "");
}
int main(int argc, char *argv[])
{
	KERNEL.SetDebugStat(false);
	argment args;
	args.load(argc, argv);
	args.for_each(proc);
	cout << input << " -> " << output << endl;
	if (input.empty() || password.empty())
	{
		Help();
		return 0;
	}
	RETURN_STAT ret;
	if (output.empty() && !decrypt)
		output = input + ".ert4";
	if (decrypt)
		ret=decrtpt_to_file(input, output, password, (output == "-"),Progress);
	else
		ret=crypt_to_file(input, output, password, algr, 0, extfile, bs,Progress);
	if (ret != OK)
	{
		printf("Library return Error: 0x%08d", ret);
	}
	return 0;
}

//LOADDEF(_main, NULL);