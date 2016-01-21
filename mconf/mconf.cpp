// mconf.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


string conffile;
string dot_config;
string out_kconf;
#include "loaddef.h"
int main(int argc, char *argv[])
{
	main_menu.section = "main";
	main_menu.display_name = "Main";
	DEBUG << "start!" << endl;
	for (int n = 0; n < argc; n++)
		if (argv[n][0]=='-')
			switch (argv[n][1])
			{
			case 'i':
				conffile = argv[n] + 2;
				break;
			case 'd':
				dot_config = argv[n] + 2;
				break;
			case 'o':
				out_kconf = argv[n] + 2;
				break;
			}
	compile(conffile);
	Prompt(out_kconf);

}

