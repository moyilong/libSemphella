// xCPU.cpp : �������̨Ӧ�ó������ڵ㡣
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