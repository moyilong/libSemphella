// cryptutil.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


string cn;
string na;
string tx;

string output;
string input;

string lic;

enum mode {
	xcrypt,
	create,
	check,
}mod;

license lics;

void init_lic()
{
	ifstream in;
	in.open(lic.data());
	if (!in.is_open())
	{
		cout << "打开文件失败!" << endl;
		abort();
	}
	char buff[DUMP_LEN];
	in.read(buff, DUMP_LEN);
	in.close();
	lics.load(buff);
}

void create_fun()
{
	cout << "Creating License..." << endl;
	lics.create(cn, na, tx);
	cout << "Dumping License...." << endl;
	string dump = lics.dump();
	//cout << "FloatPoint:" << getsum(dump.data(), dump.size()) << endl;
	float val = getsum(dump.data(), dump.size());
	printf("FloatPoint: %f\n", val);
	ofstream out;
	cout << "Writting to Files..." << endl;
	cout << "STD->" << output << endl;
	out.open(output.data());
	if (!out.is_open())
	{
		cout << "打开文件失败!" << endl;
		return;
	}
	cout << "LIC->" << output << endl;
	out.write(dump.data(), dump.size());
	cout << "END!!" << endl;
	out.close();
	
	exit(0);
}

int main(int argc,char *argv[])
{
	if (argc == 0)
	{
		cout << "Error of Args!" << endl;
		return 0;
	}
	for (int n = 0; n < argc; n++)
		if (argv[n][0]=='-' || argv[n][0]== '/')
			switch (argv[n][1])
			{
			case 'c':
				cn = argv[n] + 2;
				break;
			case 'n':
				na = argv[n] + 2;
				break;
			case 't':
				tx = argv[n] + 2;
				break;
			case 'o':
				output = argv[n] + 2;
				break;
			case 'i':
				input = argv[n] + 2;
				break;
			case 'l':
				lic = argv[n] + 2;
				break;
			case '-':
				switch (argv[n][2])
				{
				case 'c':
					mod = create;
					break;
				case 't':
					mod = check;
					break;
				case 'p':
					mod = xcrypt;
					break;
				}
				break;
			default:
				cout << "未知指令:" << argv[n] << endl;
				abort();
				break;
			}
	ifstream fin;
	ofstream fout;
	int64_t count;
	int64_t bsize,val;

	switch (mod)
	{
	case create:
		create_fun();
		break;
	case check:
		break;
	case xcrypt:
		init_lic();
		fin.open(input.data());
		fout.open(output.data());
		if (!fin.is_open()||!fout.is_open())
		{
			cout << "打开操作文件失败!" << endl;
			abort();
		}
		count = fin.tellg();
		fin.seekg(ios_base::end);
		count -= fin.tellg();
		fin.seekg(ios::beg);
		count = abs(count);
		bsize = count / MAX_BUFF_SIZE;
		val = count - (MAX_BUFF_SIZE * bsize);
		for (int n = 0; n < bsize;n++)
		{
			char buff[MAX_BUFF_SIZE];
			fin.read(buff, MAX_BUFF_SIZE);
			lics.crypt(buff, MAX_BUFF_SIZE);
			fout.write(buff, MAX_BUFF_SIZE);
		}
		if (val != 0)
		{
			char buff[MAX_BUFF_SIZE];
			fin.read(buff, MAX_BUFF_SIZE);
			lics.crypt(buff, MAX_BUFF_SIZE);
			fout.write(buff, MAX_BUFF_SIZE);
		}
		fin.close();
		fout.close();
		break;
	default:
		cout << "请指定工作模式!" << endl;
		break;
	}
	return 0;
}

