// elmgr.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "parser.h"
#include "command.h"
void CLI()
{
	while (kernel().loop_continue == true)
	{
		cout << kernel().work_file << ":>";
		string cmd;
		cin >> cmd;
		parser(cmd);
	}
}

void main(int argc, char *argv[])
{

}