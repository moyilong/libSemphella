// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>
#undef max
#undef min
#include <limits>
#include "algorthim.h"
static int64_t bs = 4096;
 bool decrypt = false;
static bool crack = false;
//static char matrix[MATRIX_LEN][MATRIX_LEN];

static bool std_out = false;
#define cp2 debug<<"[crypt2]"
#define FILE_TYPE ".ert2"
//static const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,/.<>?;':\"[] {}|\\-+*/";
#define strtbl "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

//static const int level = 2;
//static const int level_compact = 2;
#define level 2
#define level_compact 2

#ifdef LOW_PERFORMANCE_DEVICE
#define DEFAULT_ALGRTHOM_TYPE	2
#else
#define DEFAULT_ALGRTHOM_TYPE	3
#endif

#define DEBUG debug
#define MAX_PASSWORD_LEN	MAX_BUFF_SIZE

bool force = false;

struct MP_BLOCK {
	int ilen;
	double iops;
	uint64_t count = 0;
	string temp;
	uint64_t n = 0;
	double per;
};

vector<ALGHRTHIM> APOLL;


#define APOLL_SIZE	(APOLL.size())
#define APOLL_IDMAX	(APOLL_SIZE-1)
struct HEAD {
	char account_level=level;
	char algrthom;
	uint64_t sum;
	uint64_t matrix_sum;
	uint64_t password_sum;
	uint64_t bs = bs;
	inline bool check()
	{
		cp2 << "Head Level:" << (int)account_level << endl;
		cp2 << "Algrthom Type:" << (int)algrthom << endl;
		cp2 << "File Check Sum:" << sum << endl;
		cp2 << "Password Matrix Sum:" << matrix_sum << endl;
		cp2 << "Password Check Sum:" << password_sum << endl;
		cp2 << "Block Length:" << bs << endl;
		if (account_level > level || account_level < level_compact)
		{
			cout << "Error: HEAD Protoco Check Fail d!" << endl;
			cout << "Unsupported Level:" << (int)account_level << endl;
			cout << "Compact of:" << level_compact <<" max  "<<level<< endl;
			return false;
		}
		if (algrthom>APOLL_IDMAX)
		{
			cout<<"Error: HEAD Protoco Define unexist Algrthom!"<<endl;
			cout<<(int)algrthom<<"is not exist!"<<endl;
			return false;
		}
		return true;
	}
	inline HEAD()
	{
		account_level = level;
		algrthom = 0;
		sum = 0;
		matrix_sum = 0;
		password_sum = 0;
		bs = 0;
	}
};

//#define WHITE_CRYPT
uint64_t GetMatrixSum(HEAD head)
{
	return NULL;
}

void FileProcess(HEAD head, file in, file out,uint64_t &sum,int len,uint64_t op_addr)
{
	if (decrypt)
	{
		in.seekp(op_addr+sizeof(HEAD));
		if (!std_out)
			out.seekp(op_addr);
	}
	else {
		in.seekp(op_addr);
		out.seekp(op_addr + sizeof(HEAD));
	}
	char *buff = (char*)malloc(len);
	in.read(buff, len);
	uint64_t vsu = 0;
	if (!decrypt)
		vsu = APOLL.at(trans_id(head.algrthom)).sa(buff, len);
	int doff = 0;
	if (decrypt)
		doff = sizeof(HEAD);
#ifndef WHITE_CRYPT
	APOLL.at(trans_id(head.algrthom)).ca( buff, len, in.tellp() - len - doff);
#endif
	if (decrypt)
		vsu = APOLL.at(trans_id(head.algrthom)).sa(buff, len);
	if (!std_out)
		out.write(buff, len);
	else
//		cout<<buff;
		fprintf(stdout,"%s",buff);
	sum += vsu;
	free(buff);
}

inline void logo()
{
	KERNEL.LogoPrint();
	cout << "CryptUtils Version 2.0.2 " << endl << "Head Protoco Version:" << level << endl;
}

bool crack_get = false;
bool info_get = false;
int alghtriom = DEFAULT_ALGRTHOM_TYPE;
//#define ALLOW_WINDOWS_RUN
int main(int argc, char *argv[])
{
	//KERNEL.SetDebugStat(false);
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

	int al;
	string input;
	string output;
	string password;
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
	{
		if (input.empty())
		{
			cout << "Argment Error!" << endl;
		}
		else {
			if (input.substr(input.size()-5)!=".ert2")
			{
				cout<<"File Name Secure Check Faild!"<<endl;
				return false;
			}
			ifstream in;
			in.open(input.data());
			if (!in.is_open())
			{
				cout << "Open File Faid!" << endl;
				exit(-1);
			}
			HEAD head;
			cout << hex;
			in.read((char*)&head, sizeof(head));
			head.check();
			cout << "Password Checksum:" << head.password_sum << endl;
			cout << "Matrix Checksum:" << head.matrix_sum << endl;
			cout << "Checksum:" << head.sum << endl;
			cout << "Block size:" << head.bs << endl;
			cout << "Algorithm:" << (int)head.algrthom << endl;
			if (crack_get)
			{
				uint64_t count = 0;
				time_t missing = time(0);
				MP_BLOCK *block = new MP_BLOCK[omp_get_num_procs()];
				for (uint64_t v = 0; v < numeric_limits<uint64_t>::max(); v += omp_get_num_procs())
				{
#pragma omp parallel
				{
					int id = omp_get_thread_num();
					uint64_t n = v + id;
					block[id].count++;
					char str[MAX_BUFF_SIZE];
					eitoa((uint64_t)n, str, strlen(strtbl), strtbl);
					if (APOLL.at(trans_id(head.algrthom)).px(password) == head.password_sum)
					{
						cout << "Password Match! " << str << endl;
						APOLL.at(trans_id(head.algrthom)).pa(str);
						uint64_t _matrix_sum = GetMatrixSum(head);
						if (_matrix_sum == head.matrix_sum)
						{
							cout << "Password is Found!" << endl;
							cout << str << endl;
							cout << "Timeout:" << time(0) - missing << endl;
							exit(0);
						}
					}

					if (block[id].count % 1000000 == 0)
					{
						block[id].ilen = strlen(str);
						if (time(0) - missing > 0)
							block[id].iops = count / (time(0) - missing);
						else
							block[id].iops = -1;
						block[id].temp = str;
						block[id].n = n;
						block[id].per = (double)strfind(strtbl, str[0], true) / strlen(strtbl);
					}
				}
				if (block[0].count % 1000000 == 0 && block[0].count > 10000000)
				{
					uint64_t ccc = 0;
					double max = 0;
					for (int x = 0; x < omp_get_num_procs(); x++)
					{
						cout << "[" << x << "][";
						cout << " iLen:" << int2s(block[x].ilen) << " Temp:\"" << block[x].temp << "\" ";
						cout << "] ";
						max = emax(max, block[x].per);
						ccc += block[x].count;
					}
					cout << endl;
					double iops = ccc / (time(0) - missing);
					string dis = "iLen:" + int2s(block[0].temp.size()) + " IOPS:" + ull2s(iops);
					ShowProcessBar(max, dis);
					cout << "\r";
				}

				/*str_add(str);*/
				}
			}
			exit(0);
		}
	}
	if (input.empty() || password.empty())
	{
		cout << "Argment Error!" << endl<<"Input or Password Empty!"<<endl;
		exit(-1);
	}
	if (output.empty())
	{
		if (!decrypt)
		{
			output = input + FILE_TYPE;
		}
		else {
			cout << "Not define output file!" << endl;
			exit(-1);
		}
	}
	if (output == "-")
	{
		if(decrypt)
			std_out = true;
		else
		{
			cout << "Error STD Out is unsupport in Crypt Method!" << endl;
			return false;
		}
	}
	uint64_t count = 0;
	string check = output;
	if (decrypt)
		check = input;
	DEBUG << "Checking Name Secure" << endl;
	if (check.size()>5 && check != "-")
	if ((check.substr(check.size() - 5) != FILE_TYPE) || check.size()<=5)
	{
		cout << "Warring! File Name Secure Check Error!" << endl;
		exit(-1);
	}
	if (password.size() > MAX_PASSWORD_LEN)
	{
		cout << "Password is too long!" << endl;
		exit(-1);
	}
	DEBUG << "Try to Open File!" << endl;
	file in;
	file out;
	in.open(input.data(), "r");
	if (!std_out)
		out.open(output.data(), "w");
	HEAD head;
	head.account_level = level;
	head.algrthom = alghtriom;
	uint64_t len = in.tell_len();
	if (!in.is_open() )
	{
		cout << "Input File Faild!" << endl;
		exit(-1);
	}
	if (!std_out && !out.is_open())
	{
		cout << "Out File Faild!" << endl;
		exit(false);
	}
	DEBUG << "preparing data...." << endl;
	if (!decrypt)
	{
		DEBUG << "Caculating Information.." << endl;
		head.bs = bs;
		head.password_sum = APOLL.at(trans_id(head.algrthom)).px(password);
		DEBUG << "Alloc File Space" << endl;
		out.write((char*)&head, sizeof(HEAD));
	}
	else
	{
		cp2 << "Reading Head...." << endl;
		in.seekp(0);
		len = len - sizeof(HEAD);
		char buff[sizeof(HEAD)];
		in.read(buff, sizeof(HEAD));
		memcpy(&head, buff, sizeof(HEAD));
		//in.read((char*)&head, sizeof(HEAD));
		DEBUG_LINE display_dump(buff, sizeof(HEAD));
		bs = head.bs;
		cp2 << "HEAD is read!" << endl;
		if (!head.check())
		{
			cout << "Protoco Secure Check Faild!" << endl;
			cout << "Read Protoco Version:" << (unsigned int)head.account_level << endl;
			cout << "Compact Protoco Version:" << (unsigned int)level << endl;
			exit(-1);
		}
	}
	if (!std_out)	cout << input << " => " << output << endl;
	cp2 << len << " of " << bs << endl;
	cp2 << "Creating Password Matrix..." << endl;
	APOLL.at(trans_id(head.algrthom)).pa(password);
	if (decrypt)
	{
		if (GetMatrixSum(head) != head.matrix_sum || APOLL.at(trans_id(head.algrthom)).px(password) != head.password_sum)
		{
			cout << "Password Correct!" << endl;
			if (!force)
				exit(-1);
		}
	}
	cp2<<"Matrix is Created!"<<endl;
	uint64_t sum = 0;
	time_t start = time(0);
	char *buff = (char*)malloc(sizeof(char)*bs);
	if (APOLL.at(trans_id(head.algrthom)).sa == NULL || APOLL.at(trans_id(head.algrthom)).ca == NULL || APOLL.at(trans_id(head.algrthom)).pa == NULL)
	{
		cout << "Program PTR Check Error!" << endl;
		exit(-1);
	}
	uint64_t step = len / head.bs;
	uint64_t fix = len - (head.bs * step);
	cp2 << "Resetting Address..." << endl;
	if (decrypt)
	{
		in.seekp(sizeof(HEAD));
		if (!std_out)
		out.seekp(0);
	}
	else {
		in.seekp(0);
		out.seekp(sizeof(HEAD));
	}
	double old_presend = 0,dlen=len;
	uint64_t ulen = 0;
	for (uint64_t n = 0; n < step; n++)
	{
		FileProcess(head, in, out, sum, head.bs, n*head.bs);
		if (!std_out)
		{
			double per = (double)((double)n*(double)head.bs) / (double)len;
			if (per != old_presend)
			{
				old_presend = per;
				ulen = (n* head.bs) / dZero(time(0) - start);
				ShowProcessBar(per, human_read(ulen,human_read_storage_str,1024,10)  + "PS");
			}
		}
	}
	FileProcess(head, in, out, sum, fix,step*head.bs);
	cp2 << "Main Loop Over! SUM:" << sum << endl;
	if (!std_out) ShowProcessBar(1, "--");
	cout << endl;
	if (!decrypt)
	{
		cp2 << "updating head..." << endl;
		head.sum = sum;
		head.algrthom = alghtriom;
		head.account_level = level;
		cp2 << "Caculating Matrix SUM" << endl;
		head.matrix_sum = GetMatrixSum(head);
		cp2 << "Redirecting..." << endl;
		out.seekp(0);
		cp2 << "Redirect to " << out.tellp() << endl;
		cp2 << "Writing Data..." << endl;
		head.check();
		out.write((char*)&head, sizeof(HEAD));
		DEBUG_LINE display_dump((char*)&head, sizeof(HEAD));
		cp2 << "head is updated!" << endl;
	}
	else
	{
		if (sum != head.sum)
		{
			cout << "Checksum Faild!" << endl;
			cout << hex << sum << " != " << hex << head.sum << endl;
		}
	}
	if (!std_out) cout << "Done. Checksum:" << hex << sum << endl;
	in.close();
	if (!std_out)
	out.close();
}
