// MSHELL.cpp : �������̨Ӧ�ó������ڵ㡣
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
	cout << "1.\t�޸�ROOT����" << endl;
	cout << "2.\t������繲���û�" << endl;
	cout << "3.\t��ʽ������" << endl;
	cout << "��ѡ��:";
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
			cout << "�������û���:" << endl;
			cin >> tmpstr;
			tmpstr = +"smbpasswd -a ";
			system(tmpstr.data());
			break;
		case 3:
			cout << "�������ʽ��Ŀ��:" << endl;
			cin >> tmpstr;
			cout << "�������ʽ���ļ�ϵͳ:" << endl;
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
			cout << "Ϊָֹ��!" << endl;
			break;
		}
	}

}