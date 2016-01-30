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
int64_t bs = 16384;
bool decrypt = false;
bool crack = false;

#define cp2 debug<<"[crypt2]"

const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,/.<>?;':\"[] {}|\\-+*/";

void str_add(char *g)
{
	bool nex_plus = true;
	for (int n = 0; n < MAX_BUFF_SIZE; n++)
	{
		if (g[n] == '\0'&&nex_plus)
		{
			g[n] = strtbl[0];
			nex_plus = false;
			continue;
		}
		if (g[n] == '\0')
			continue;
		int now = -1;
		for (int x = 0; x < strlen(strtbl); x++)
			if (g[n] == strtbl[x])
				now = x;
		if (now == -1)
			continue;
		if (nex_plus)
		{
			if (g[n] == strtbl[strlen(strtbl) - 1])
			{
				nex_plus = true;
				g[n] = strtbl[n];
				continue;
			}
			else {
				g[n] = strtbl[now + 1];
				nex_plus = false;
				continue;
			}
		}

	}
}

char matrix[MATRIX_LEN][MATRIX_LEN];
inline uint64_t GetMatrixSum()
{
	uint64_t matrix_sum[MATRIX_LEN];
#pragma omp parallel for
	for (int n = 0; n < MATRIX_LEN; n++)
		matrix_sum[n] = getsumV2(matrix[n], MATRIX_LEN*sizeof(char));
	return getsumV2((char*)matrix_sum, sizeof(uint64_t)*MATRIX_LEN);
}



#define DEBUG if (false)	cout<<__FILE__<<"@"<<__LINE__<<" ::"
#define MAX_PASSWORD_LEN	MAX_BUFF_SIZE

struct HEAD {
	int64_t bs = bs;
	uint64_t sum;
	uint64_t matrix_sum;
	uint64_t password_sum;
};
#include "mpblock.h"
bool crack_get = false;
bool info_get = false;
int main(int argc, char *argv[])
{

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
			cout << "Password Checksum:" << head.password_sum << endl;
			cout << "Matrix Checksum:" << head.matrix_sum << endl;
			cout << "Checksum:" << head.sum << endl;
			cout << "Block size:" << head.bs << endl;
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
						if (getsumV2(str, strlen(str)) == head.password_sum)
						{
							cout << "Password Match! " << str << endl;
							CreateMatrix(str, matrix);
							uint64_t _matrix_sum = GetMatrixSum();
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
	if (!decrypt)
	{
		head.bs = bs;
		head.password_sum = getsumV2(password.data(), password.size());
		out.write((char*)&head, sizeof(HEAD));
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
	CreateMatrix(password, matrix);
	if (decrypt)
	{
		if (GetMatrixSum() != head.matrix_sum||getsumV2(password.data(),password.size())!=head.password_sum)
		{
			cout << "Password Correct!" << endl;
			exit(-1);
		}
	}
	uint64_t sum = 0;
	for (uint64_t n = 0; n+bs < len; n += bs)
	{
		count++;
		char *buff = (char*)malloc(sizeof(char)*bs);
		memset(buff, 0, sizeof(buff));
		in.read(buff, bs);
		if (!decrypt)
		sum += getsumV2(buff, bs);
		xor_cryptV2(matrix, buff, bs, bs*count);
		if (decrypt)
			sum += getsumV2(buff, bs);
		out.write(buff, bs);
		free(buff);
		if (count%5 == 0)
			ShowProcessBar((double)count*bs / len, "");
	}
	if (len - bs*count > 0)
	{
		uint64_t fix = len - bs*count;
		char *buff = new char[bs];
		memset(buff, 0, sizeof(buff));
		in.read(buff, fix);
		if (!decrypt)
		sum += getsumV2(buff, fix);
		xor_cryptV2(matrix, buff, fix, bs*count);
		if (decrypt)
			sum += getsumV2(buff, fix);
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
		head.matrix_sum = GetMatrixSum();
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