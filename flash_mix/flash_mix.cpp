// flash_mix.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/libbios.h>
#include <libSemphella/crypt.h>
#include <libSemphella/string.h>
string PASSWORD = "moyiypt_passwog_c&lrd::^&*$^#nor$#";

const char *hextbl = "0123456789ABCDEF";

inline float get_acc_float(const float input)
{
	float ret= input * 10 + sin(input) - cos(input) * tan(input);
	return ret;
}

int ehoti(const char* str)
{
	if (strstr(str, "0x") || strstr(str, "0X"))
	{
		str += 2;
		int tmp = 0;
		int len = strlen(str);
		int i = 0;
		for (i = 0; i < len; i++)
		{
			int nDecNum;
			switch (str[i])
			{
			case 'a':
			case 'A':
				nDecNum = 10;
				break;
			case 'b':
			case 'B':
				nDecNum = 11;
				break;
			case 'c':
			case 'C':
				nDecNum = 12;
				break;
			case 'd':
			case 'D':
				nDecNum = 13;
				break;
			case 'e':
			case 'E':
				nDecNum = 14;
				break;
			case 'f':
			case 'F':
				nDecNum = 15;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				nDecNum = str[i] - '0';
				break;
			default:
				return 0;

			}
			tmp += nDecNum*pow(16, len - i - 1);
		}
		return tmp;
	}
	else
	{
		return atoi(str);
	}
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
		info.start = ehoti(xline.substr(ipoll.at(0) + 1, ipoll.at(1)).data());
		info.len = ehoti(xline.substr(ipoll.at(1),ipoll.at(2)).data());
		info.origin = xline.substr(ipoll.at(2));
		poll.push_back(info);
	}
}
PARTITION_INFO Str2Par(string in)
{
	//STD_FORMAT partition_name:origin_file.start+len
	int emp=-1;
	int spl = -1;
	int dot = -1;
	emp = strfind(in.data(), '.', true);
	spl = strfind(in.data(), ':', true);
	dot = strfind(in.data(), '+', true);
	if (emp == -1 || spl == -1 || dot == -1)
	{
		cout << "Synatx Error:" << in << endl;
		exit(-1);
	}
	PARTITION_INFO ret;
	ret.pname = in.substr(0, spl);
	ret.origin = in.substr(spl + 1, emp);
	ret.start = ehoti(in.substr(emp + 1, dot).data());
	ret.len = ehoti(in.substr(dot + 1).data());
	return ret;
}
int bios_part = 0;
string output = "firmware.efi";
#define MB	1024*1024
unsigned long long fsize = 16 * MB;
int vendor = 0xAF5D;
int product = 0xBF9E;
void main(int argc, char *argv[])
{
	bool std_input = false;
	for (int n = 1; n < argc; n++)
		if (argv[n][0] == '-')
			switch (argv[n][1])
		{
			case 'i':
				poll.push_back(Str2Par(argv[n] + 2));
				break;
			case 'f':
				read_from_file(argv[n] + 2);
				break;
			case 'o':
				output = argv[n] + 2;
				break;
			case 'n':
				bios_part = ehoti(argv[n] + 2);
				break;
			case 'l':
				fsize = ehoti(argv[n] + 2);
				break;
			case 'P':
				PASSWORD = argv[n] + 2;
				break;
			case 's':
				std_input = true;
				break;
			case 'v':
				vendor = ehoti(argv[n] + 2);
				break;
			case 'p':
				product = ehoti(argv[n] + 2);
				break;
			default:
				cout << "未知命令行:" << argv[n] << endl;
				exit(-1);
				break;
		}
	cout << "优华电器 Sirius 量产以及SPI固件合成工具" << endl;
	cout << "Vendor:" << hex<<vendor <<oct<< " Product:" << hex<<product <<oct<< endl;
	if (vendor > 0xFFFF || product > 0xFFFF)
	{
		cout << "Vendor or Product ID Error!" << endl;
		exit(-1);
	}
	cout << "请输入密码:";
	string pwd;
	cin >> pwd;
	cout << endl;
	float passwd = get_acc_float(getsum(pwd.data(), strlen(pwd.data())));
	if (get_acc_float(38.281021) != passwd&&get_acc_float(39.504578) != passwd)
	{
		cout << "警告:错误的密码!" << endl;
		abort();
	}
	cout << "密码确认!" << endl;
	if (std_input)
		while (true)
		{
			cout << "STD:" << endl;
			string in;
			cin >> in;
			if (streval("::END", in.data()))
				break;
			poll.push_back(Str2Par(in));
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
			bios.info.vendor = vendor;
			bios.info.product = product;
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