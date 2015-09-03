// dCodec.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/libSemphella.h>
#include <libSemphella/crypt.h>
#include <libSemphella/efstream.h>
char work_mode = 'd';
string work_file;
string out_file;

ifstream input;
ofstream output;

string password = "moyilongmoyilongmoyilongmoyilong";


void process(unsigned long long process_len)
{
	char *buff = (char*)malloc(sizeof(char)*process_len);
	sZero(buff, 0, process_len);
	input.read(buff, process_len);
	crypt(buff, process_len, password);
	output.write(buff, process_len);
	free(buff);
	buff = NULL;
}

int main(int argc, char *argv[])
{
	for (int n = 1; n < argc; n++)
	{
		if (argv[n][0] == '-')
		{
			switch (argv[n][1])
			{
			case 'd':
			case 'e':
				work_mode = argv[n][1];
				break;
			case 'f':
				n++;
				work_file = argv[n];
				break;
			case 'o':
				n++;
				out_file = argv[n];
				break;
			case 'p':
				n++;
				password = argv[n];
				break;
			default:
				cout << "unknow command!" << endl;
				return -1;
				break;
			}
		}
		else {
			cout << "unknow command at:" << n << " == " << argv[n] << endl;
			exit(-1);
		}
	}

	input.open(work_file.data());
	output.open(out_file.data());
	if (!input.is_open() || !output.is_open())
	{
		cout << "file open faild!" << endl;
		return false;
	}
	fpos_t file_len = filelen(input);
	unsigned long long cp_count = file_len / (MAX_BUFF_SIZE*sizeof(char));
	unsigned long long last_count = file_len - cp_count * (MAX_BUFF_SIZE*sizeof(char));
	DEBUG_LINE{
		cout <<"file len:"<<file_len<<endl<<"CP Count:" << cp_count << endl << "Last Count:" << last_count << endl;
	}
	input.seekg(0);
	for (unsigned long long n = 0; n < cp_count; n++)
		process(MAX_BUFF_SIZE);
	process(last_count);
	input.close();
	output.close();
	
}