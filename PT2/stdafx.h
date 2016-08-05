// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

// TODO:  在此处引用程序需要的其他头文件
#include <libSemphella/main.h>
#include <libSemphella/string.h>
#include <libSemphella/debug.h>
#include <libSemphella/sysapi_modcall.h>
#include <libSemphella/crypt.h>
#include <libSemphella/utils.h>

struct sysconfig{
	int time = 5;
	int based_offset = 0;
	uint64_t begin=0;
	uint64_t stop=0;
	bool un_excepted = false;
};

extern sysconfig cfg;
extern vector<int> poll;
