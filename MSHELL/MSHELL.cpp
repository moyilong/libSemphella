// MSHELL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#ifdef __LINUX__
#define BANNER_FILE	"/etc/banner"
#else
#define BANNER_FILE	"banner"
#endif

vector<string>banner;

int menu()
{
	for (int n = 0; n < banner.size(); n++)
		cout << banner.at(n)<<endl;
	cout << "1.\t修改ROOT密码" << endl;
	cout << "2.\t添加网络共享用户" << endl;
	cout << "3.\t格式化分区" << endl;
	cout << "请选择:";
	string in;
	cin >> in;
	return atoi(in.data());
}

int main(int argc, char *argv[])
{
	ifstream ibanner;
	ibanner.open(BANNER_FILE);
	if (ibanner.is_open())
	{
		while (!ibanner.eof())
		{
			string read;
			getline(ibanner, read);
			banner.push_back(read);
		}
	}
	string tmpstr;
	string tmpstr2;
	string tmpstr3;
	while (true)
	{
		tmpstr = "";
		switch (menu())
		{
		case 1:
			system("passwd");
			break;
		case 2:
			cout << "请输入用户名:" << endl;
			cin >> tmpstr;
			tmpstr = +"smbpasswd -a ";
			system(tmpstr.data());
			break;
		case 3:
			cout << "请输入格式化目标:" << endl;
			cin >> tmpstr;
			cout << "请输入格式化文件系统:" << endl;
			cin >> tmpstr3;
			tmpstr2 = "umount ";
			tmpstr2 += tmpstr;
			system(tmpstr2.data());
			tmpstr2 = tmpstr;
			tmpstr = "mkfs.";
			tmpstr += tmpstr3;
			tmpstr += " ";
			tmpstr += tmpstr2;
		default:
			cout << "为止指令!" << endl;
			break;
		}
	}

}