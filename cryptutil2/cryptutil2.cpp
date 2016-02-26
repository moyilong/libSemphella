// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cryptutil2.h"
#include <libSemphella/argment.h>

 int64_t bs = 4096;
 bool decrypt = false;
 bool crack = false;
 bool std_out = false;


bool force = false;
vector<ALGHRTHIM> APOLL;
bool license_create = false;
bool crack_get = false;
bool info_get = false;
int alghtriom = DEFAULT_ALGRTHOM_TYPE;
//#define ALLOW_WINDOWS_RUN

int al;
string input;
string output;
string password;

void logo()
{
	KERNEL.LogoPrint();
	cout << "CryptUtils Version 3.0.0-Alpha " << endl << "Head Protoco Version:" << level << endl;
	cout << CORE_NAME << endl << "API Level:" << ull2s(API_VER) << endl << "Max Buff Size:" << ull2s(MAX_BUFF_SIZE) << endl;
}

bool file_name_check(string filename)
{
	cp2 << "Check File Name\"" << filename << "\"" << endl;
	if (filename.size() <= strlen(".ert3"))
		return false;
	if (streval(filename.substr(filename.size() - 5).data(), ".ert2"))
		return true;
	if (streval(filename.substr(filename.size() - 5).data() , ".ert3"))
		return true;
	return false;
}

bool HEAD::check()
{
	cp2 << "Head Level:" << (int)account_level << endl;
	cp2 << "Algrthom Type:" << (int)algrthom << endl;
	cp2 << "File Check Sum:" << sum << endl;
	//cp2 << "Password Matrix Sum:" << matrix_sum << endl;
	cp2 << "Password Check Sum:" << password_sum << endl;
	cp2 << "Block Length:" << bs << endl;
	if (account_level > level || account_level < level_compact)
	{
		cout << "Error: HEAD Protoco Check Fail d!" << endl;
		cout << "Unsupported Level:" << (int)account_level << endl;
		cout << "Compact of:" << level_compact << " max  " << level << endl;
		return false;
	}
	if (algrthom>APOLL_IDMAX)
	{
		cout << "Error: HEAD Protoco Define unexist Algrthom!" << endl;
		cout << (int)algrthom << "is not exist!" << endl;
		return false;
	}
	return true;
}
HEAD::HEAD()
{
	account_level = level;
	algrthom = 0;
	sum = 0;
	matrix_sum = 0;
	password_sum = 0;
	bs = 0;
}

void config_read(string name, string value)
{
		switch (name.at(0)) {
			case 'i':
				input = value;
				break;
			case 'o':
				output = value;
				break;
			case 'p':
				password = value;
				break;
			case 'd':
				decrypt = true;
				break;
			case 'f':
				force = true;
				break;
			case 'b':
				bs = atoi(value.data());
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
				break;
			case 'V':
				logo();
				cout << "Max Algrthon Type: 0~" << APOLL_IDMAX << endl;
				cout << "Defined Block Size:" << bs << endl;
				if (KERNEL.GetDebugStat())
					cout << "Verbos is Enabled!" << endl;
				cout << "Deafult Algrthom ID " << DEFAULT_ALGRTHOM_TYPE << endl;
				exit(0);
				break;
			case 'l':
				license_create = true;
				break;

			}
}

int main(int argc, char *argv[])
{
	//KERNEL.SetDebugStat(false);
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
	cp2<<"MAX Algrthon Type: 0~"<<APOLL_IDMAX<<endl;
	cp2<<"Deafult Algrthom ID "<<DEFAULT_ALGRTHOM_TYPE<<endl;

	argment config_load;
	cp2 << "Import Argment..." << endl;
	config_load.load(argc, argv);
	cp2 << "Process Argment by foreach" << endl;
	config_load.for_each(config_read);
	cp2 << "Init Funcation" << endl;
	if (license_create)
		return create_license();
	if (info_get)
		return information_process();
	return crypt_process();
}