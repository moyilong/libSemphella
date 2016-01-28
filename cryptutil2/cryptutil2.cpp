// cryptutil2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
vector<ARGMENT>poll;

int64_t bs = 16384;

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
	if (!HaveArgment("b", poll))
		bs = atoi(GetValue("b", poll).data());
	uint64_t len = get_file_len(input.data());
	uint64_t count = 0;
	ifstream in;
	ofstream out;
	in.open(input.data());
	out.open(output.data());
	if (!in.is_open() || !out.is_open())
	{
		cout << "Open File Faild!" << endl;
		exit(-1);
	}
	for (uint64_t n = 0; n < len; n += bs)
	{
		count++;
		char *buff = (char*)malloc(sizeof(char)*bs);
		memset(buff, 0, sizeof(buff));
		in.read(buff, bs);
		xor_cryptV2(password, buff, bs, bs*count);
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
		xor_cryptV2(password, buff, fix, bs*count);
		out.write(buff, fix);
		free(buff);
	}
	in.close();
	out.close();
}