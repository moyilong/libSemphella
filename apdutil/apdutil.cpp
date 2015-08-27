// apdutil.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace APD_UTILS;


int main(int argc, char *argv[])
{
	APD test("test.txt");
	test.set_bin_mode(true);
	test.set_password("moyilong");
	test.set_crypt_mode(true);
	test.export_bin();
}