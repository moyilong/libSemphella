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
int64_t bs = 4096;
bool decrypt = false;
bool crack = false;

#define cp2 debug<<"[crypt2]"

const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,/.<>?;':\"[] {}|\\-+*/";

char matrix[MATRIX_LEN][MATRIX_LEN];

const int level = 2;
const int level_compact = 2;

#define DEBUG if (false)	cout<<__FILE__<<"@"<<__LINE__<<" ::"
#define MAX_PASSWORD_LEN	MAX_BUFF_SIZE

struct HEAD {
	char account_level=level;
	char algrthom=0;
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
			cout << "Error: HEAD Protoco Check Faild!" << endl;
			cout << "Unsupported Level:" << (int)account_level << endl;
			cout << "Compact of:" << level_compact <<" max  "<<level<< endl;
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

typedef void (*password_algrthom)(string password, char matrix[MATRIX_LEN][MATRIX_LEN]);
typedef void(*crypt_algrthom)(char matrix[MATRIX_LEN][MATRIX_LEN], char *data, int64_t len, int64_t bit_off);
typedef uint64_t(*sum_algrthom)(const char *data, int64_t len);

struct ALGRHOM {
	password_algrthom pa;
	crypt_algrthom ca;
	sum_algrthom sa;
	inline ALGRHOM(password_algrthom p, crypt_algrthom c,sum_algrthom s)
	{
		pa = p;
		ca = c;
		sa = s;
	}
};
string _stored_pwd;
void CreateMatrix_NULL(string password, char matrix[MATRIX_LEN][MATRIX_LEN])
{
	_stored_pwd = password;
#pragma omp parallel for
	for (int n = 0; n < MATRIX_LEN; n++)
		for (int x = 0; x < MATRIX_LEN; x++)
			matrix[n][x] = 0;
}

void CryptAlgrthom(char matrix[MATRIX_LEN][MATRIX_LEN], char *data, int64_t len, int64_t bit_off)
{
	xor_crypt(_stored_pwd, data, len);
}


const ALGRHOM APOLL[] = {
	{ CreateMatrix,xor_cryptV2_1,getsumV2 },
	{ CreateMatrix,xor_cryptV2,getsumV2 },
	{ CreateMatrix_NULL,CryptAlgrthom,getsumV2 },
};

#define APOLL_SIZE	(sizeof(APOLL) / sizeof(ALGRHOM))
#define APOLL_IDMAX	(APOLL_SIZE-1)
//#define WHITE_CRYPT
uint64_t GetMatrixSum(HEAD head)
{
	uint64_t matrix_sum[MATRIX_LEN];
#pragma omp parallel for
	for (int n = 0; n < MATRIX_LEN; n++)
		matrix_sum[n] = APOLL[head.algrthom].sa(matrix[n], MATRIX_LEN*sizeof(char));
	return  APOLL[head.algrthom].sa((char*)matrix_sum, sizeof(uint64_t)*MATRIX_LEN);
}

void FileProcess(HEAD head, file in, file out,uint64_t &sum,int len,uint64_t op_addr)
{
	if (decrypt)
	{
		in.seekp(op_addr+sizeof(HEAD));
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
		vsu = APOLL[head.algrthom].sa(buff, len);
	int doff = 0;
	if (decrypt)
		doff = sizeof(HEAD);
#ifndef WHITE_CRYPT
	APOLL[head.algrthom].ca(matrix, buff, len, in.tellp() - len - doff);
#endif
	if (decrypt)
		vsu = APOLL[head.algrthom].sa(buff, len);
	out.write(buff, len);
	sum += vsu;
	free(buff);
}

namespace SECURE_CHECK {
	char buff[MATRIX_LEN][MATRIX_LEN];
	char data[MATRIX_LEN];
	inline void SC()
	{
		CreateMatrix("test_test_tst", buff);
#pragma omp parallel for
		for (int n = 0; n < MATRIX_LEN; n++)
			data[n] = rand();
		uint64_t orig = getsumV2(data, MATRIX_LEN);
		xor_cryptV2_1(buff, data, MATRIX_LEN, 0);
		uint64_t cpd = getsumV2(data, MATRIX_LEN);
		xor_cryptV2_1(buff, data, MATRIX_LEN, 0);
		uint64_t dcp = getsumV2(data, MATRIX_LEN);
		if (orig == cpd || dcp == cpd || orig != dcp)
		{
			cout << "Secure Check Faild!" << endl;
			exit(-1);
		}
	}
}

#include "mpblock.h"
bool crack_get = false;
bool info_get = false;
int alghtriom = 0;
#define ALLOW_WINDOWS_RUN
int main(int argc, char *argv[])
{
	KERNEL.LogoPrint();
	cout << "CryptUtils Version 2.0.1 " << endl << "Head Protoco Version:" << level << endl;
#ifndef __LINUX__
	cout << "Error:This Program is can't run in windows !" << endl;
	cout << "      Please use linux version!" << endl;
#ifdef ALLOW_WINDOWS_RUN
	exit(-1);
#else
	cout << "Running Windows System Secure Check!" << endl;
	SECURE_CHECK::SC();
#endif
#endif
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
				if (al > APOLL_IDMAX)
				{
					cout << "Max Algorithm ID is " << APOLL_IDMAX << endl;
					exit(-1);
				}
				alghtriom = al;
				break;
			case 'v':
				KERNEL.SetDebugStat(true);
				break;
			}
	if (info_get)
	{
		if (input.empty())
		{
			cout << "Argment Error!" << endl;
		}
		else {
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
					if (APOLL[head.algrthom].sa(str, strlen(str)) == head.password_sum)
					{
						cout << "Password Match! " << str << endl;
						APOLL[head.algrthom].pa(str, matrix);
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
		cout << "Argment Error!" << endl;
		exit(-1);
	}
	if (output.empty())
	{
		if (!decrypt)
		{
			output = input + ".ert2";
		}
		else {
			cout << "Not define output file!" << endl;
			exit(-1);
		}
	}
	uint64_t count = 0;
	string check = output;
	if (decrypt)
		check = input;
	DEBUG << "Checking Name Secure" << endl;
	if (check.substr(check.size() - 5) != ".ert2")
	{
		cout << "Warring! File Name Secure Check Error!" << endl;
		exit(-1);
	}
	if (password.size() > MAX_PASSWORD_LEN)
	{
		cout << "Password is too long!" << endl;
		exit(-1);
	}
	file in;
	file out;
	in.open(input.data(), "r");
	out.open(output.data(), "w");
	HEAD head;
	head.account_level = level;
	head.algrthom = alghtriom;
	uint64_t len = in.tell_len();
	if (!in.is_open() || !out.is_open())
	{
		cout << "Open File Faild!" << endl;
		exit(-1);
	}
	if (!decrypt)
	{
		head.bs = bs;
		head.password_sum = APOLL[head.algrthom].sa(password.data(), password.size());
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
	cout << input << " => " << output << endl;
	cout << len << " of " << bs << endl;
	cp2 << "Creating Password Matrix..." << endl;
	APOLL[head.algrthom].pa(password, matrix);
	if (decrypt)
	{
		if (GetMatrixSum(head) != head.matrix_sum || APOLL[head.algrthom].sa(password.data(), password.size()) != head.password_sum)
		{
			cout << "Password Correct!" << endl;
			exit(-1);
		}
	}
	uint64_t sum = 0;
	time_t start = time_t(0);
	char *buff = (char*)malloc(sizeof(char)*bs);
	if (APOLL[head.algrthom].sa == NULL || APOLL[head.algrthom].ca == NULL || APOLL[head.algrthom].pa == NULL)
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
		out.seekp(0);
	}
	else {
		in.seekp(0);
		out.seekp(sizeof(HEAD));
	}
	double old_presend = 0;
	for (uint64_t n = 0; n < step; n++)
	{
		FileProcess(head, in, out, sum, head.bs, n*head.bs);
		double per = ((double)n*(double)head.bs) / (double)len;
		if (per != old_presend)
		{
			old_presend = per;
			ShowProcessBar(per, ull2s((n* head.bs) / (time_t(0) - start)) + "/S");
		}
	}
	FileProcess(head, in, out, sum, fix,step*head.bs);
	cp2 << "Main Loop Over! SUM:" << sum << endl;
	ShowProcessBar(1, "--");
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
	cout << "Done. Checksum:" << hex << sum << endl;
	in.close();
	out.close();
}