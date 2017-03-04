// xCPU.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/secload.h>
int _main(int argc, char *argv[])
{
	init();
	loop();
	return 0;
}

LOADDEF(_main, NULL);