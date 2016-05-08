// s2s.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/crypt.h>
#include <time.h>
#include <math.h>
const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz~`!@#$%^&*()-=_+{}[]\\|;':\",.<>/?/+*-";
#include <libSemphella/sum.h>

#include <limits>
#include <libSemphella/utils.h>
#include <libSemphella/files.h>
#include <libSemphella/argment.h>

int main(int argc, char *argv[])
{
	if (streval("-f", argv[1]))
	{
		file load;
		load.open(argv[2], "r");
		if (!load.is_open())
		{
			cout << "Faild Open File!" << endl;
			return -1;
		}
		printf("%x", load.getsum());
		exit(0);
	}
	string arg_t;
	for (int n = 0; n < argc; n++)
		arg_t += argv[n];
	uint64_t ret = getsumV2(arg_t.data(), arg_t.size());
	printf("%08ull", ret);
	return 0;
}