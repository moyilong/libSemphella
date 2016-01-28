// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
vector<ARGMENT>poll;

int64_t bs = 16384;
bool decrypt = false;

char matrix[MATRIX_LEN][MATRIX_LEN];


struct HEAD {
	int64_t bs = bs;
	uint64_t sum;
};

int main(int argc, char *argv[])
{
	ProcessArgment(argc, argv, poll);
	string input = GetValue("i", poll);
	string output = GetValue("o", poll);
	string password = GetValue("p", poll);
	if (!HaveArgment("i", poll) || !HaveArgment("o", poll) || !HaveArgment("p", poll))
	{
		cout << "Argment Error!" << endl;
		exit(-1);
	}	
	decrypt = HaveArgment("d", poll);
	if (!HaveArgment("b", poll))
		bs = atoi(GetValue("b", poll).data());
	uint64_t len = get_file_len(input.data());
	if (decrypt)
		len -= sizeof(HEAD);
	uint64_t count = 0;
	string check = output;
	if (decrypt)
		check = input;
	if (check.substr(-4) != ".ert2")
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
	CreateMatrix(password, matrix);
	uint64_t sum = 0;
	for (uint64_t n = 0; n < len; n += bs)
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
		if (n%bs == 100)
			ShowProcessBar((double)count*bs / len, "");
	}
	if (len - bs*count > 0)
	{
		uint64_t fix = len - bs*count;
		char *buff = (char*)malloc(fix);
		memset(buff, 0, sizeof(buff));
		in.read(buff, fix);
		if (!decrypt)
		sum += getsumV2(buff, bs);
		xor_cryptV2(matrix, buff, fix, bs*count);
		if (decrypt)
			sum += getsumV2(buff, bs);
		out.write(buff, fix);
		free(buff);
	}
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
	in.close();
	out.close();
}