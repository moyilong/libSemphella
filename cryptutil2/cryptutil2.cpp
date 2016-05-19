// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cryptutil2.h"
#include <libSemphella/argment.h>
#include <libERT/libERT.h>
#include <libERT/ext_io.h>
#include <libSemphella/date_recovery_plus.h>
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
//bool load_ext_info = false;
//char *buff = nullptr;
#include <libSemphella/math.h>
void config_read(string name, string value)
{
	int itemp;
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
	case 'j':
		omp_set_num_threads(atoi(value.data()));
		break;
	case 'd':
		mode = DECRYPT;
		break;
	case 'P':
		PerformanceTest();
		exit(0);
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
			exit(-1);
		}
		break;
	case 'A':
		al = atoi(value.data());
		if (get_alg_id(al) == -1)
		{
			cout << "Faild to Get Algorghim!" << endl;
			exit(-1);
		}
		debug << "Set ID To" << al << endl;
		break;
	case 'v':
		KERNEL.SetDebugStat(true);
		fs_verbos(true);
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
		exit(0);
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
		exit(0);
	case 'l':
		mode = LICENSE_CREATE;
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
			exit(0);
		}
		if (streval(temp.data(), "aestest"))
		{
			aesTest();
			exit(0);
		}
		if (streval(temp.data(), "drptest"))
		{
			uint64_t val = time(0);
			uint64_t len = LIM_RAND_ULD(4096, 16384);
			cout << val << " => " << len << endl;
			if (!TrustyTestPerFrame(val, len))
			{
				cout << "Faild!" << endl;
			}
			exit(0);
		}
		if (streval(temp.data(), "drpstreet"))
		{
			uint64_t all=0;
			uint64_t fin = 0;
			uint64_t beg = time(0);
			while (true)
			{
				all++;
				uint64_t val = time(0);
				uint64_t len = LIM_RAND_ULD(4096, 16384);
				//cout << val << " => " << len << endl;
				if (TrustyTestPerFrame(val, len))
					fin++;
				if (all % 5 == 0)
				{
					cout << "                                         " << (double)(fin * 100 / all) << "% " << (all - fin) << ":" << all << " " << (double)(all / dZero((time(0) - beg))) << " fps  \r";
				}
			}
		}
		break;
	}
}

int main(int argc, char *argv[])
{
	pre_calc_pct(4096);
	//KERNEL.SetDebugStat(false);
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
	HEAD head;
	RETURN_STAT stat = OK;
	switch (mode)
	{
	case FILE_INFO:
		if (!file_name_check(input))
		{
			cout << "Input Name Check Faild!" << endl;
			exit(-1);
		}
		if (input.empty())
		{
			cout << "Input is Empty!" << endl;
			exit(-1);
		}
		head = get_head(input);
		break;
	case CRYPT:
		if (input.empty())
		{
			cout << "Input is Empty!" << endl;
			exit(-1);
		}
		if (output.empty())
			output = input + ".ert4";
		if (!file_name_check(output))
		{
			cout << "File name Check Faild!" << endl;
			exit(-1);
		}
		stat = crypt_to_file(input, output, password, al, fhand, ext_file, bs);
		break;
	case DECRYPT:
		if (!file_name_check(input))
		{
			cout << "File name Check Faild!" << endl;
			exit(-1);
		}
		if (input.empty())
		{
			cout << "Input is Empty!" << endl;
			exit(-1);
		}
		if (!std_mode)
			if (output.empty())
			{
				cout << "Output is Empty!" << endl;
				exit(-1);
			}
		stat = decrtpt_to_file(input, output, password, std_mode);
		break;
	case LICENSE_CREATE:
		if (!std_mode)
			if (output.empty())
			{
				cout << "Output is Empty!" << endl;
				exit(-1);
			}
		return create_license(output, std_mode, bs);
		break;
	case EXT_TO_FILE:
		if (!file_name_check(input))
		{
			cout << "File name Check Faild!" << endl;
			exit(-1);
		}
		if (input.empty())
		{
			cout << "Input is Empty!" << endl;
			exit(-1);
		}
		if (output.empty())
		{
			cout << "Output is Empty!" << endl;
			exit(-1);
		}
		stat = get_ext_to_file(input, output, std_mode);
		break;
	}
	if (stat != OK)
	{
		cout << "Faild to Run Program!" << endl;
		cout << "API Return ID:" << stat << endl;
		return -1;
	}
	return 0;
}