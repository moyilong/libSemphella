// InsightServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "include.h"
#include "utils.h"

bool daemon = false;

int main(int argc, char *argv[])
{
	init();
	for (int n = 1; n < argc; n++)
	{
		if (argv[n][0] == '-')
			switch (argv[n][1])
		{
			case 'd':
				daemon = true;
				break;
		}
	}
	if (daemon)
		server_main();
	else
		client_main();
}