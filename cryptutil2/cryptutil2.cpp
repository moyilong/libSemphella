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
int64_t bs = 16384;
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
	inline void check()
	{
		if (account_level > level || account_level < level_compact)
		{
			cout << "Error: HEAD Protoco Check Faild!" << endl;
			cout << "Unsupported Level:" << (int)account_level << endl;
			cout << "Compact of:" << level_compact <<" max  "<<level<< endl;
		}
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


ALGRHOM APOLL[] = {
	//{PAM_1,CAM_1,SUM_1},
	{CreateMatrix,xor_cryptV2_1,getsumV2},
	{ CreateMatrix,xor_cryptV2,getsumV2 },

};

#define APOLL_SIZE	(sizeof(APOLL) / sizeof(ALGRHOM))
#define APOLL_IDMAX	(APOLL_SIZE-1)

uint64_t GetMatrixSum(HEAD head)
{
	uint64_t matrix_sum[MATRIX_LEN];
#pragma omp parallel for
	for (int n = 0; n < MATRIX_LEN; n++)
		matrix_sum[n] = APOLL[head.algrthom].sa(matrix[n], MATRIX_LEN*sizeof(char));
	return  APOLL[head.algrthom].sa((char*)matrix_sum, sizeof(uint64_t)*MATRIX_LEN);
}



#include "mpblock.h"
bool crack_get = false;
bool info_get = false;
int alghtriom = 0;
int main(int argc, char *argv[])
{
	
	KERNEL.LogoPrint();
	cout << "CryptUtils Version 2.0.1 "<<endl<<"Head Protoco Version:"<<level << endl;
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
					int al = atoi(argv[n]);
					if (al > APOLL_IDMAX)
					{
						cout << "Max Algorithm ID is " << APOLL_IDMAX << endl;
						exit(-1);
					}
					alghtriom = al;
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
				//char str[MAX_BUFF_SIZE];
				//memset(str, '\0', MAX_BUFF_SIZE);
				//str[0] = strtbl[0];
				uint64_t count = 0;
				time_t missing = time(0);
				MP_BLOCK *block = new MP_BLOCK[omp_get_num_procs()];
				for (uint64_t v = 0; v < numeric_limits<uint64_t>::max(); v+=omp_get_num_procs())
				{
#pragma omp parallel
					{
						int id = omp_get_thread_num();
						uint64_t n = v+id;
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
	if (input.empty() || output.empty() || password.empty())
	{
		cout << "Argment Error!" << endl;
		exit(-1);
	}
	DEBUG << "Get File Len..." << endl;
	//uint64_t len = get_file_len(input.data());
	FILE *fp = fopen(input.data(), "r");
	uint64_t len = ftell(fp);
	fseek(fp, 0, SEEK_END);
	len = ftell(fp) - len;
	fclose(fp);
	if (decrypt)
		len -= sizeof(HEAD);
	uint64_t count = 0;
	string check = output;
	if (decrypt)
		check = input;
	DEBUG << "Checking Name Secure" << endl;
	if (check.substr(check.size()-5) != ".ert2")
	{
		cout << "Warring! File Name Secure Check Error!" << endl;
		exit(-1);
	}
	if (password.size() > MAX_PASSWORD_LEN)
	{
		cout << "Password is too long!" << endl;
		exit(-1);
	}
	ifstream in;
	ofstream out;
	in.open(input.data());
	out.open(output.data());
	HEAD head;
	head.account_level = level;
	head.algrthom = alghtriom;
	if (!decrypt)
	{
		head.bs = bs;
		head.password_sum = APOLL[head.algrthom].sa(password.data(), password.size());
		out.write((char*)&head, sizeof(HEAD));
		head.check();
	}
	else
	{
		char buff[sizeof(HEAD)];
		in.read(buff, sizeof(HEAD));
		memcpy(&head, buff, sizeof(HEAD));
		bs = head.bs;
	}
	if (!in.is_open() || !out.is_open())
	{
		cout << "Open File Faild!" << endl;
		exit(-1);
	}
	cout << input <<  " => " << output << endl;
	cout << len << " of " << bs << endl;
	APOLL[head.algrthom].pa(password, matrix);
	if (decrypt)
	{
		if (GetMatrixSum(head) != head.matrix_sum|| APOLL[head.algrthom].sa(password.data(),password.size())!=head.password_sum)
		{
			cout << "Password Correct!" << endl;
			exit(-1);
		}
	}
	uint64_t sum = 0;
	time_t start=time_t(0);
	for (uint64_t n = 0; n+bs < len; n += bs)
	{
		count++;
		char *buff = (char*)malloc(sizeof(char)*bs);
		memset(buff, 0, sizeof(buff));
		in.read(buff, bs);
		if (!decrypt)
		sum += APOLL[head.algrthom].sa(buff, bs);
		APOLL[head.algrthom].ca(matrix, buff, bs, bs*count);
		if (decrypt)
			sum += APOLL[head.algrthom].sa(buff, bs);
		out.write(buff, bs);
		free(buff);
		if (count%5 == 0)
		{
			uint64_t cp_len = (n*bs) / dZero(time(0)-start);
		#ifdef __LINUX__
			cp_len /=1000;
		#endif
			ShowProcessBar((double)count*bs / len, human_read(cp_len,human_read_storage_str,1024,10)+ "/S");
		}
	}
	if (len - bs*count > 0)
	{
		uint64_t fix = len - bs*count;
		char *buff = new char[bs];
		memset(buff, 0, sizeof(buff));
		in.read(buff, fix);
		if (!decrypt)
		sum += APOLL[head.algrthom].sa(buff, fix);
		APOLL[head.algrthom].ca(matrix, buff, fix, bs*count);
		if (decrypt)
			sum += APOLL[head.algrthom].sa(buff, fix);
		out.write(buff, fix);
		delete[]buff;
	}
	ShowProcessBar(1, "");
	cout << endl;
	cp2 << "Flushing Cache..." << endl;
	out.flush();
	if (!decrypt)
	{
		cp2 << "updating head..." << endl;
		head.sum = sum;
		cp2 << "Caculating Matrix SUM" << endl;
		head.matrix_sum = GetMatrixSum(head);
		cp2 << "Redirecting..." << endl;
		out.seekp(ios_base::beg);
		cp2 << "Writing Data..." << endl;
		out.write((char*)&head, sizeof(HEAD));
		cp2 << "Flushing..." << endl;
		out.flush();
		cp2 << "head is updated!" << endl;
	}
	else
	{
		if (sum != head.sum)
		{
			cout << "Checksum Faild!" << endl;
			cout << hex<<sum << " != " <<hex<< head.sum << endl;
		}
	}
	cout << "Done. Checksum:" <<hex<< sum << endl;
	in.close();
	out.close();
}