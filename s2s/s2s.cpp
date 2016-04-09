// s2s.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/crypt.h>
#include <time.h>
#include <math.h>
const char *strtbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz~`!@#$%^&*()-=_+{}[]\\|;':\",.<>/?/+*-";
#include <libSemphella/sum.h>

ofstream out("logdump.log");
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
			cout << "Open File Faild!" << endl;
			return -1;
		}
		uint64_t len = load.tell_len();
		uint64_t steps = len / MAX_BUFF_SIZE;
		uint64_t mix = len - steps*MAX_BUFF_SIZE;
		uint64_t sum = 0;
		char buff[MAX_BUFF_SIZE];
		for (uint64_t n = 0; n < steps; n++)
		{
			memset(buff, 0, MAX_BUFF_SIZE);
			load.read(buff, MAX_BUFF_SIZE);
			sum += getsumV2(buff, MAX_BUFF_SIZE);
		}
		memset(buff, 0, MAX_BUFF_SIZE);
		load.read(buff, mix);
		sum += getsumV2(buff, mix);
		printf("%08ull", sum);
		load.close();
		return 0;
	}
	string arg_t;
	for (int n = 0; n < argc; n++)
		arg_t += argv[n];
	uint64_t ret = getsumV2(arg_t.data(), arg_t.size());
	printf("%08ull", ret);
	return 0;
}