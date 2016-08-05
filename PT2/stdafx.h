// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
