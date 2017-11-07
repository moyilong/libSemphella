// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/libSemphella.h>
#include <libSemphella/argment.h>
#include <libSemphella/string.h>
#include <libERT/libERT.h>
#include <libERT/ext_io.h>
#include <libERT/libERT.h>
/*
#define FILE_TYPE ".ert3"
#define strtbl "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

#define cp2 debug
#define DEBUG debug
#define MAX_PASSWORD_LEN	MAX_BUFF_SIZE

enum WORK_MODE {
	FILE_INFO,
	CRYPT,
	DECRYPT,
	EXT_TO_FILE,
	MASK_OUT
};

void logo()
{
	KERNEL.LogoPrint();
	cout << "CryptUtils Version 3.2.5-Beta " << endl << "Head Protoco Version:" << level << endl;
	cout << "libERT Version:" << get_api_ver() << endl;
	cout << CORE_NAME << endl << "API Level:" << ull2s(API_VER) << endl << "Max Buff Size:" << ull2s(MAX_BUFF_SIZE) << endl;
}

string input, password, output, ext_file;
int bs = 2 * 1024 * 1024, al = DEFAULT_ALG_ID, fhand = DEFAULT_FHL_ID;
bool file_name_check(string filename)
{
	cp2 << "Check File Name \"" << filename << "\"" << endl;
	cp2 << "Sub Name Is:" << filename.substr(filename.size() - 5) << endl;
	if (filename.size() <= strlen(".ert3"))
		return false;
	if (streval(filename.substr(filename.size() - 5).data(), ".ert2"))
		return true;
	if (streval(filename.substr(filename.size() - 5).data(), ".ert3"))
		return true;
	if (streval(filename.substr(filename.size() - 5).data(), ".ert4"))
		return true;
	return false;
}
bool std_mode = false;
WORK_MODE mode = CRYPT;
#include <libSemphella/math.h>
bool broken = false;
bool exited = false;
void config_read(string name, string value)
{
	int itemp;
	string temp;
	switch (name.at(0)) {
	case 'g':
		broken = true;
		break;
	case 'i':
		input = value;
		break;
	case 'o':
		output = value;
		break;
	case 'p':
		password = ull2s(getsumV2(value.data(), value.size()));
		break;
	case 'n':
		password = value;
		break;
	case 'j':
		omp_set_num_threads(atoi(value.data()));
		break;
	case 'd':
		mode = DECRYPT;
		break;
	case 'P':
		PerformanceTest();
		exited = true;
		break;
	case 'b':
		bs = atoi(value.data());
		break;
	case 'E':
		//load_ext_info = true;
		mode = EXT_TO_FILE;
		break;
	case 'e':
		ext_file = value;
		break;
	case 'I':
		mode = FILE_INFO;
		break;
	case 'm':
		if (LoadExternalLib(value) != 0)
		{
			cout << "Loadlib:" << value << " Faild" << endl;
			exited = true;
		}
		break;
	case 'A':
		al = atoi(value.data());
		if (get_alg_id(al) == -1)
		{
			cout << "Faild to Get Algorghim!" << endl;
			exited = true;
		}
		debug << "Set ID To" << al << endl;
		break;
	case 'v':
		KERNEL.SetDebugStat(true);
		break;
	case 's':
		std_mode = true;
		break;
	case 'V':
		logo();
		cout << "Defined Block Size:" << bs << endl;
		if (KERNEL.GetDebugStat())
			cout << "Verbos is Enabled!" << endl;
		cout << "Deafult Algrthom ID " << DEFAULT_ALGRTHOM_TYPE << endl;
		cout << "Extenision Table Size:" << EXT_SIZE << endl;
		cout << "Default Fhandle Size:" << DEFAULT_FHANDLE << endl;
		cout << "Alloed Fhandle ID:" << endl;
		cout << endl;
		exited = true;
		break;
	case 'a':
		logo();
		cout << "All of Algorthim and Documents!" << endl;
		cout << "==============================================" << endl;
		cout << "All of Algorthim:" << algor_max() << endl;
		cout << "Default Algorthim ID:" << al << endl;
		for (int n = 0; n < algor_max(); n++)
		{
			get_alg_info(n, itemp, temp);
			cout << n << ":" << itemp << "\t" << temp << endl;
		}
		exited = true;
	case 'x':
		mode = MASK_OUT;
		break;
	case '-':
		temp = name.substr(1);
		if (streval(temp.data(), "thread"))
		{
			debug << "setting thread number:" << atoi(value.data());
			omp_set_num_threads(atoi(value.data()));
		}
		if (streval(temp.data(), "fctest"))
		{
			fcTest();
			exited = true;
		}
		if (streval(temp.data(), "aestest"))
		{
			aesTest();
			exited = true;
		}
		if (streval(temp.data(), "decrypt"))
		{
			getsum2_decrypt(getsumV2("moyilong", strlen("moyilong")),0xFFFFFFFFF,numeric_limits<uint64_t>::max(),"abcdefghijklmnopqrstuvwxyz");
			exited = true;
		}
		if (streval(temp.data(), "mp2test"))
		{
			mpSum_Test(0, true);
		}
		if (streval(temp.data(), "mptest"))
		{
			mpSum_Test(MAX_BUFF_SIZE, false);
		}
		break;
	}
}

int _main(int argc, char *argv[])
{
	KERNEL.SetDebugStat(false);
	pre_calc_pct(4096);
#ifndef __LINUX__
	logo();
	cout << "Error:This Program is can't run in windows !" << endl;
	cout << "      Please use linux version!" << endl << endl;
#ifdef ALLOW_WINDOWS_RUN
	exit(-1);
#else
	cout << "Program is alloed be run!" << endl << endl;
#endif
#endif
	cp2 << "Deafult Algrthom ID " << DEFAULT_ALGRTHOM_TYPE << endl;

	argment config_load;
	cp2 << "Import Argment..." << endl;
	config_load.load(argc, argv);
	cp2 << "Process Argment by foreach" << endl;
	config_load.for_each(config_read);
	cp2 << "Init Funcation" << endl;
	if (exited)
		return 0;
	HEAD head;
	RETURN_STAT stat = OK;
	switch (mode)
	{
	case FILE_INFO:
		if (!file_name_check(input))
		{
			cout << "Input Name Check Faild!" << endl;
			return -1;
		}
		if (input.empty())
		{
			cout << "Input is Empty!" << endl;
			return -1;
		}
		head = get_head(input);
		if (broken)
		{
			vector<string> str = getsum2_decrypt(head.password_sum);
			for (int n = 0; n < str.size(); n++)
				cout << "\"" << str.at(n) << "\"" << endl;
		}
		break;
	case CRYPT:
		if (input.empty())
		{
			cout << "Input is Empty!" << endl;
			return -1;
		}
		if (output.empty())
			output = input + ".ert4";
		if (!file_name_check(output))
		{
			cout << "File name Check Faild!" << endl;
			return -1;
		}
		stat = crypt_to_file(input, output, password, al, fhand, ext_file, bs);
		break;
	case DECRYPT:
		if (!file_name_check(input))
		{
			cout << "File name Check Faild!" << endl;
			return -1;
		}
		if (input.empty())
		{
			cout << "Input is Empty!" << endl;
			return -1;
		}
		if (!std_mode)
			if (output.empty())
			{
				cout << "Output is Empty!" << endl;
				return -1;
			}
		stat = decrtpt_to_file(input, output, password, std_mode);
		break;
	case EXT_TO_FILE:
		if (!file_name_check(input))
		{
			cout << "File name Check Faild!" << endl;
			return -1;
		}
		if (input.empty())
		{
			cout << "Input is Empty!" << endl;
			return -1;
		}
		if (output.empty())
		{
			cout << "Output is Empty!" << endl;
			return -1;
		}
		stat = get_ext_to_file(input, output, std_mode);
		break;
	case MASK_OUT:
		char buffers[MAX_BUFF_SIZE];
		ofstream of(output);
		while (true)
		{
			randomMatch(buffers, MAX_BUFF_SIZE, DEFAULT_WORD_WHITE_LIST);
			of.write(buffers, MAX_BUFF_SIZE);
			of.flush();
		}
	}
	if (stat != OK)
	{
		cout << "Faild to Run Program!" << endl;
		cout << "API Return ID:" << stat << endl;
		return -1;
	}
	return 0;
}

//SEC_LOADER;
LOADDEF(_main,NULL);
*/
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
int main(int argc, char *argv[])
{
	argment args;
	args.load(argc, argv);
	args.for_each(proc);
	cout << input << " -> " << output << endl;
	if (input.empty() || password.empty())
	{
		Help();
		return 0;
	}
	if (output.empty() && !decrypt)
		output = input + ".ert4";
	if (decrypt)
		decrtpt_to_file(input, output, password, (output == "-"));
	else
		crypt_to_file(input, output, password, algr, 0, extfile, bs);
	return 0;
}

//LOADDEF(_main, NULL);