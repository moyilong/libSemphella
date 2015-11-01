// flash_mix.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/libbios.h>
#include <libSemphella/crypt.h>

string PASSWORD = "moyiypt_passwog_c&lrd::^&*$^#nor$#";

const char *hextbl = "0123456789ABCDEF";

inline float get_acc_float(const float input)
{
	float ret= input * 10 + sin(input) - cos(input) * tan(input);
	cout << "Return Fload:" << ret << endl;
	return ret;
}

int _htoi(const char *str)
{
	int ret = 0;
	for (int n = strlen(str) - 1; n >= 0; n++)
	{
		int off = 0;
		for (off = 0; off < strlen(hextbl); off++)
			if (str[n] == hextbl[off] || str[n] == (hextbl[off] - ('a' - 'A')))
				break;
		ret += pow(16, n)*off;
	}
	return ret;
}

int htoi(const char *str)
{
	if (str[0] == 0 && str[1] == 'x')
		return _htoi(str + 2);
	else
		return atoi(str);
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
#define MB	1024*1024
unsigned long long fsize = 16 * MB;
void main(int argc, char *argv[])
{
	cout << "Sirius 量产以及SPI固件合成工具" << endl;
	cout << "请输入密码:";
	string pwd;
	cin >> pwd;
	cout << endl;
	float passwd = get_acc_float(getsum(pwd.data(), strlen(pwd.data())));
	if (get_acc_float(38.281021) !=passwd&&get_acc_float(37.217239) !=passwd)
	{
		cout << "警告:错误的密码!" << endl;
		abort();
	}
	cout << "密码确认!" << endl;
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
				bios_part = htoi(argv[n] + 2);
				break;
			case 'l':
				fsize = htoi(argv[n] + 2);
				break;
			case 'P':
				PASSWORD = argv[n] + 2;
				break;
			default:
				cout << "未知命令行:" << argv[n] << endl;
				exit(-1);
				break;
		}
	char *firmware_size = (char*)calloc(fsize, sizeof(char));
	for (int n = 0; n < poll.size(); n++)
	{
		if (n == bios_part)
		{
			size_t iLen;
			char buff[128];
			memset(buff, 0, sizeof(buff));
			getenv_s(&iLen, buff, "TEMP");			
			string tmp_file =buff;
			tmp_file.append("//");
			memset(buff, 0, sizeof(buff));
			sprintf_s(buff, "%d%d%d", rand(), rand(), rand());
			tmp_file.append(buff);
			BIOS bios(tmp_file, "ELONE DragonOS moyilong Phenom Semphella libDragon", true);
			bios.setenv(0, rand()*rand()*rand());
			poll.at(n).origin = tmp_file;
		}
		ifstream input;
		input.open(poll.at(n).pname.data(), ios::_Nocreate);
		if (!input.is_open())
		{
			cout << "Open Original File Faild::" << poll.at(n).pname << " from " << poll.at(n).origin << endl;
			free(firmware_size);
			abort();
		}
		input.read(firmware_size + poll.at(n).start, poll.at(n).len);
		input.close();
	}
	if (!output.empty())
	{
		ofstream iout;
		iout.open(output);
		if (!iout.is_open())
		{
			cout << "Create Target File Faild!" << endl;
			free(firmware_size);
			abort();
		}
		iout.write(firmware_size, fsize);
		iout.close();
		free(firmware_size);
		return;
	}
}