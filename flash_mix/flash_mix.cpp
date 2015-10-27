// flash_mix.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/libbios.h>

#define PASSWORD	"moyilong"

const char *hextbl = "0123456789ABCDEF";

int htoi(const char *str)
{
	int ret = 0;
	for (int n = strlen(str) - 1; n >= 0; n++)
	{
		int off = 0;
		for (off = 0; off < strlen(hextbl); off++)
			if (str[n] == hextbl[off]||str[n]==(hextbl[off]-('a'-'A')))
				break;
		ret += pow(16, n)*off;
	}
	return ret;
}

struct PARTITION_INFO{
	string pname;
	int start;
	int len;
	string origin;
};
vector<PARTITION_INFO>poll;

void read_from_file(string file)
{
	ifstream io;
	io.open(file.data(), ios_base::_Nocreate);
	if (!io.is_open())
	{
		cout << "File Read Faild!" << endl;
		return;
	}
	while (!io.eof())
	{
		string xline;
		getline(io, xline);
		vector<int>ipoll;
		for (int n = 0; n < xline.size(); n++)
			if (xline.at(n) == ' ')
				ipoll.push_back(n);
		PARTITION_INFO info;
		info.pname = xline.substr(0, ipoll.at(0));
		info.start = htoi(xline.substr(ipoll.at(0) + 1, ipoll.at(1)).data());
		info.len = htoi(xline.substr(ipoll.at(1),ipoll.at(2)).data());
		info.origin = xline.substr(ipoll.at(2));
		poll.push_back(info);
	}
}
int bios_part = 0;
string output = "firmware.efi";
void main(int argc, char *argv[])
{
	for (int n = 1; n < argc; n++)
		if (argv[n][0] == '-')
			switch (argv[n][1])
		{
			case 'p':
				if (true)
				{
					PARTITION_INFO info;
					info.pname = argv[n] + 2;
					n++;
					info.start = htoi(argv[n]);
					n++;
					info.len = htoi(argv[n]);
					n++;
					info.origin = argv[n];
					poll.push_back(info);
				}
				break;
			case 'f':
				read_from_file(argv[n] + 2);
				break;
			case 'o':
				output = argv[n] + 2;
				break;
			case 'n':
				bios_part = atoi(argv[n] + 2);
				break;
			default:
				break;
		}
	ofstream out;
	out.open(output.data(), ios::_Noreplace);
	if (!out.is_open())
	{
		cout << "OpenFile Faild!" << endl;
		return;
	}
	out.seekp(out.beg);
	for (int n = 0; n < poll.size(); n++)
	{
		ifstream ipart;
		ipart.open(poll.at(n).origin.data(), ios::_Nocreate);
		if (!ipart.is_open())
		{
			cout << "Open Partition File Faild!" << endl;
			cout << n << "@" << poll.at(n).pname << endl;
			abort();
		}
		char *buff = (char*)calloc(poll.at(n).len, sizeof(char));
		ipart.readsome(buff, poll.at(n).len);
		out.write(buff, poll.at(n).len);
		ipart.close();
		free(buff);
	}
}