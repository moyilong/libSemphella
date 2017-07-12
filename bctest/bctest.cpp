// bctest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libBroadCast/libBroadCast.h>
#include <libSemphella/main.h>
#include <iostream>
#include <string>
using namespace std;
void RC(const char *ptr)
{
	cout << "recv:" << ptr << endl;
}

void HS(char *ptr, int &len)
{
	ptr = (char *)malloc(128);
	strcpy(ptr, "moyilong_test");
	len = 128;
}

void HP(const char *head, char *resend, int &len)
{
	cout << "head:" << head << endl;
	resend = (char *)malloc(128);
	strcpy(resend, "message recived!");
	len = 128;
}

ProcdConfigure cfg;

int main(int argc,char *argv[])
{
	cfg.recv = RC;
	cfg.hs = HS;
	cfg.hp = HP;
	if (argv[1][1] != 'l')
	{
		StartListenDaemon(65540, cfg);
	}
	else
	{
		SendBroadcast(65540, 10, cfg);
	}
}

