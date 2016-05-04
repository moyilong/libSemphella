// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cryptutil2.h"
#include <libSemphella/argment.h>
extern int timeout;
int64_t bs = 4096;
bool decrypt = false;
bool crack = false;
bool std_out = false;

bool force = false;
bool license_create = false;
bool crack_get = false;
bool info_get = false;
int alghtriom = DEFAULT_ALGRTHOM_TYPE;
extern int fhand = DEFAULT_FHANDLE;
//#define ALLOW_WINDOWS_RUN

int al;
string input;
string output;
string password;
#include "fhandle.h"
#define MAX_EXDATA_SIZE	16384

void load_ext_data(string filename)
{
	file ext;
	ext.open(filename,"r");
	if (ext.tell_len() > MAX_EXDATA_SIZE)
	{
		cout << "Error: External File is too large!" << endl;
		exit(-1);
	}
	ex.length = ext.tell_len();
	ext_data = (char*)malloc(ex.length);
	memset(ext_data, 0, sizeof(ext_data));
	ext.read(ext_data, ex.length);
	ext.close();
}


void logo()
{
	KERNEL.LogoPrint();
	cout << "CryptUtils Version 3.2.5-Beta " << endl << "Head Protoco Version:" << level << endl;
	cout << CORE_NAME << endl << "API Level:" << ull2s(API_VER) << endl << "Max Buff Size:" << ull2s(MAX_BUFF_SIZE) << endl;
}

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
	return false;
}

bool HEAD::check()
{
	cp2 << "Head Level:" << (int)account_level << endl;
	cp2 << "Algrthom Type:" << (int)algrthom << endl;
	cp2 << "File Handle Type:" << (int)ext[EXT_FHANDLE] << endl;
	cp2 << "File Check Sum:" << sum << endl;
	cp2 << "Password Check Sum:" << password_sum << endl;
	cp2 << "Block Length:" << bs << endl;
	cp2 << "Extenision Table Size:" << EXT_SIZE << endl;
	if (account_level > level || account_level < level_compact)
	{
		cout << "Error: HEAD Protoco Check Fail d!" << endl;
		cout << "Unsupported Level:" << (int)account_level << endl;
		cout << "Compact of:" << level_compact << " max  " << level << endl;
		return false;
	}
	if (algrthom > APOLL_IDMAX)
	{
		cout << "Error: HEAD Protoco Define unexist Algrthom!" << endl;
		cout << (int)algrthom << "is not exist!" << endl;
		return false;
	}
	if (ext[EXT_SUPPORT] != ext_support_lab || ext[EXT_ENDFLAG] != ext_end_lab)
	{
		cout << "Warring: HEAD Protoco Extend Table Check Faild!" << endl;
		cout << " Convert to Default Status!" << endl;
		cout << hex << "CheckLab:" << (int)(char)ext_support_lab << " != " << (int)(char)ext[EXT_SUPPORT] << endl << oct;
		reset_ext();
	}
	return true;
}
EXT ex;
void HEAD::reset_ext()
{
	cp2 << "Resetting Extension Table..." << endl;
	memset(ext, 0, sizeof(ext));
	ext[EXT_FHANDLE] = DEFAULT_FHANDLE;
	ext[EXT_SUPPORT] = ext_support_lab;
	ext[EXT_EXTABLE] = 1;
	ext[EXT_ENDFLAG] = ext_end_lab;
}
HEAD::HEAD()
{
	account_level = level;
	algrthom = 0;
	sum = 0;
	password_sum = 0;
	bs = 0;
	reset_ext();
}
bool load_ext_info = false;
char *buff = nullptr;
void config_read(string name, string value)
{
	string temp;
	bool stat = false;
	switch (name.at(0)) {
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
	case 'd':
		decrypt = true;
		break;
	case 'f':
		force = true;
		break;
	case 'P':
		PerformanceTest();
		exit(0);
		break;
	case 'b':
		bs = atoi(value.data());
		break;
	case 'E':
		load_ext_info = true;
		break;
	case 'e':
		load_ext_data(value);
		break;
	case 'I':
		info_get = true;
		break;
	case 'C':
		info_get = true;
		crack_get = true;
		break;
	case 'A':
		al = atoi(value.data());
		cp2 << "Resetting Algorithm ID " << alghtriom << " => " << al << endl;
		if (trans_id(al) == -1)
		{
			cout << "Warrong ID!" << endl;
			exit(-1);
		}
		alghtriom = al;
		break;
	case 'v':
		KERNEL.SetDebugStat(true);
		fs_verbos(true);
		break;
	case 'V':
		logo();
		cout << "Defined Block Size:" << bs << endl;
		if (KERNEL.GetDebugStat())
			cout << "Verbos is Enabled!" << endl;
		cout << "Deafult Algrthom ID " << DEFAULT_ALGRTHOM_TYPE << endl;
		cout << "Extenision Table Size:" << EXT_SIZE << endl;
		cout << "Fhandle Size:" << fsize + 1 << endl;
		cout << "Default Fhandle Size:" << DEFAULT_FHANDLE << endl;
		cout << "Alloed Fhandle ID:" << endl;
		for (int n = 0; n < fsize; n++)
			cout << FPOLL[n].id << "\t";
		cout << endl << "Allowed Algrthom ID:" << endl;
		for (int n = 0; n < xsize; n++)
			cout << APOLL[n].id << "\t";
		cout << endl;
		exit(0);
		break;
	case 'l':
		license_create = true;
		break;
	case 'F':
		al = atoi(value.data());
		for (int x = 0; x < fsize; x++)
			if (FPOLL[x].id == al)
				stat = true;
		if (!stat)
		{
			cout << "File Handle Not Exist!" << endl;
			exit(0);
		}
		cp2 << "Resetting File Handle" << fhand << " => " << al << endl;
		fhand = al;
		break;
	case '-':
		temp = name.substr(1);
		if (streval(temp.data(), "timeout"))
			timeout = atoi(value.data());
		if (streval(temp.data(), "thread"))
		{
			debug << "setting thread number:" << atoi(value.data());
			omp_set_num_threads(atoi(value.data()));
		}
		if (streval(temp.data(), "fctest"))
		{
			fcTest();
			exit(0);
		}
		break;
	}
}

int main(int argc, char *argv[])
{
	KERNEL.SetDebugStat(false);
#ifndef __LINUX__
	logo();
	cout << "Error:This Program is can't run in windows !" << endl;
	cout << "      Please use linux version!" << endl;
#ifdef ALLOW_WINDOWS_RUN
	exit(-1);
#else
	cout << "Program is alloed be run!" << endl;
#endif
#endif
	cp2 << "MAX Algrthon Type: 0~" << APOLL_IDMAX << endl;
	cp2 << "Deafult Algrthom ID " << DEFAULT_ALGRTHOM_TYPE << endl;

	argment config_load;
	cp2 << "Import Argment..." << endl;
	config_load.load(argc, argv);
	cp2 << "Process Argment by foreach" << endl;
	config_load.for_each(config_read);
	cp2 << "Init Funcation" << endl;
	if (load_ext_info)
	{
		ext_dump();
		exit(0);
	}
	if (license_create)
		return create_license();
	if (info_get)
		return information_process();
	return crypt_process();
}