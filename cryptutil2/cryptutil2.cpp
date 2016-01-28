// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>

int64_t bs = 16384;
bool decrypt = false;

char matrix[MATRIX_LEN][MATRIX_LEN];

#define DEBUG	cout<<__FILE__<<"@"<<__LINE__<<" ::"

struct HEAD {
	int64_t bs = bs;
	uint64_t sum;
};

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
	ifstream in;
	ofstream out;
	in.open(input.data());
	out.open(output.data());
	HEAD head;
	if (!decrypt)
	out.write((char*)&head, sizeof(HEAD));
	else
	{
		in.read((char*)&head, sizeof(HEAD));
		bs = head.bs;
	}
	if (!in.is_open() || !out.is_open())
	{
		cout << "Open File Faild!" << endl;
		exit(-1);
	}
	cout << input <<  "=> " << output << endl;
	cout << len << " of " << bs << endl;
	CreateMatrix(password, matrix);
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
		//if (n/bs == 10)
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
	cout << endl;
	if (!decrypt)
	{
		out.seekp(0);
		out.write((char*)&head, sizeof(HEAD));
	}
	else
	{
		if (sum != head.sum)
		{
			cout << "Checksum Faild!" << endl;
		}
	}
	cout << "Checksum:" <<hex<< sum << endl;
	in.close();
	out.close();
}