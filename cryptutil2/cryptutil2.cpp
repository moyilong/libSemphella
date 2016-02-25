// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cryptutil2.h"

 int64_t bs = 4096;
 bool decrypt = false;
 bool crack = false;
 bool std_out = false;


bool force = false;
vector<ALGHRTHIM> APOLL;

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
}

bool file_name_check(string filename)
{
	return input.substr(input.size() - 5) != ".ert2"&&input.substr(input.size() - 5) != ".ert3";
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

int main(int argc, char *argv[])
{
	KERNEL.SetDebugStat(false);
#ifndef __LINUX__
	logo();
	cout << "Error:This Program is can't run in windows !" << endl;
	cout << "      Please use linux version!" << endl;
#ifdef ALLOW_WINDOWS_RUN
	exit(-1);
#endif
#endif
	cp2<<"MAX Algrthon Type: 0~"<<APOLL_IDMAX<<endl;
	cp2<<"Deafult Algrthom ID "<<DEFAULT_ALGRTHOM_TYPE<<endl;

	for (int n = 0; n < argc; n++)
		if (argv[n][0] == '-')
			switch (argv[n][1]) {
			case 'i':
				n++;
				input = argv[n];
				break;
			case 'o':
				n++;
				output = argv[n];
				break;
			case 'p':
				n++;
				password = argv[n];
				break;
			case 'd':
				decrypt = true;
				break;
			case 'f':
				force = true;
				break;
			case 'b':
				n++;
				bs = atoi(argv[n]);
				break;
			case 'I':
				info_get = true;
				break;
			case 'C':
				info_get = true;
				crack_get = true;
				break;
			case 'A':
				n++;
				al = atoi(argv[n]);
				cp2<<"Resetting Algorithm ID "<<alghtriom <<" => "<<al<<endl;
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
				cout<<"Max Algrthon Type: 0~"<<APOLL_IDMAX<<endl;
				cout<<"Defined Block Size:"<<bs<<endl;
				if (KERNEL.GetDebugStat())
					cout<<"Verbos is Enabled!"<<endl;
				cout<<"Deafult Algrthom ID "<<DEFAULT_ALGRTHOM_TYPE<<endl;
				return true;
				break;
				
			}

	if (info_get)
		return information_process();
	crypt_process();
}
