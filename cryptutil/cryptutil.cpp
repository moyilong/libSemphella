// cryptutil.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/crypt.h>
#include <libSemphella/AES.h>
#include "common.h"
#include <libSemphella/sum.h>
#define CHECK_STR	"ELONE_DRAGONOS_ABCDEF"

string password;
string infile;
string outfile;
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_SIZE	4096
AES aes;
bool enctype_stat = true;
FILE *input;
FILE *output;

unsigned char real_time_sha1[ZEN_SHA1_HASH_SIZE];

inline void process(FILE *in, FILE *out, size_t plen)
{
	unsigned char *read_buff = (unsigned char *)malloc(plen*sizeof(unsigned char));
	memset(read_buff, 0, sizeof(read_buff));
	fread(read_buff, sizeof(unsigned char), plen / sizeof(unsigned char), in);
	if (enctype_stat)
        aes.Encrypt(read_buff, plen);
	else
        aes.Decrypt(read_buff, plen);
    fwrite(read_buff, sizeof(unsigned char), plen / sizeof(unsigned char), out);
	free(read_buff);
}

int main(int argc, char *argv[])
{
	for (int n = 0; n < argc; n++)
	if (argv[n][0]=='-')
		switch (argv[n][1])
		{
		case 'p':
			password = argv[n] + 2;
			break;
		case 'i':
			infile = argv[n] + 2;
			break;
		case 'o':
			outfile = argv[n] + 2;
			break;
		case 'd':
			enctype_stat = false;
			break;
		}
	if (streval(infile.data(), "-"))
	{
		if (outfile.empty())
		{
			cout << "Out File Name is Empty!" << endl;
			return -1;
		}
		output=fopen(outfile.data(), "wb+");
		if (output == NULL)
		{
			cout << "Open Write Back File Faild!" << endl;
			return -1;
		}
		unsigned char buff[BLOCK_SIZE];
		bool conn = true;
		while (conn)
		{
			int count = -1;
			for (int n = 0; n < BLOCK_SIZE; n++)
			{
				int tmpget = getchar();
				buff[n] = tmpget;
				if (tmpget == EOF)
				{
					count = n;
					break;
				}
			}
			if (count == -1)
				count = BLOCK_SIZE;
			if (enctype_stat)
				aes.Encrypt(buff, count);
			else
				aes.Decrypt(buff, count);
			fwrite(buff, sizeof(unsigned char), count, output);
			memset(buff, 0, sizeof(buff));
		}
		
	}
	if (password.empty() || infile.empty() || outfile.empty())
		exit(-1);
	input = fopen(infile.data(), "rb+");
	output = fopen(outfile.data(), "wb+");
	if (input == NULL || output == NULL)
	{
		cout << "File Operation Faild!" << endl;
		if (input!=NULL)
			fclose(input);
		if (output != NULL)
			fclose(output);
		exit(-2);
	}
	unsigned char *pdata = (unsigned char *)malloc(password.size()*sizeof(unsigned char));
	memcpy(pdata, password.data(), password.size()*sizeof(unsigned char));
	free(pdata);
	long long len = ftell(input);
	fseek(input, 0, SEEK_END);
	len = ftell(input) - len;
	fseek(input, 0, SEEK_SET);
	long long count = 0;
	cout << "Work Stat:" << hex<<len << " per " <<hex<< BLOCK_SIZE << endl;
	cout << "Process " << infile << " => " << outfile << endl;
	for (int n = 0; n +BLOCK_SIZE< len;n+=BLOCK_SIZE)
	{
		count++;
		process(input, output, BLOCK_SIZE);
	}
	if (count*BLOCK_SIZE < len)
	{
		cout << "Special Fix :" << len - (count*BLOCK_SIZE)<<endl;
		process(input, output, len - (count*BLOCK_SIZE));
	}
exit:
	fclose(input);
	fclose(output);
}
