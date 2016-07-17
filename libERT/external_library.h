#pragma once
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>
#include "headproto.h"
typedef void(*password_algrthom)(string password);	//��ʼ������
typedef void(*crypt_algrthom)(char *data, int64_t len, int64_t bit_off);	//�����㷨
typedef uint64_t(*sum_algrthom)(const char *data, int64_t len);	//У����㷨
typedef uint64_t(*get_password_checksum)(string passwd);	//����У���㷨
typedef void(*CFHANDLE)(HEAD head, file _in, file _out, uint64_t &sum, int len, uint64_t op_addr, bool decrypt, bool std_out, int mps);	//�ļ��������

struct algr_t {
	bool can_be_pt = true;
	//�����ʼ���㷨
	password_algrthom pa;
	//�����㷨
	crypt_algrthom ca;
	int id;
	//У��ֵ�㷨
	sum_algrthom sa;
	//����У���㷨
	get_password_checksum px;
	//˵��
	string doc;
};

struct fhand_t {
	CFHANDLE hand;
	int id;
};

struct modules_info {
	string name;
	fhand_t *handle_type;
	algr_t *algr_type;
	int fhand_size = 0;
	int algr_size = 0;
	inline modules_info(string _name, fhand_t *fptr, algr_t *algr, int fs, int as);
};

inline modules_info::modules_info(string _name, fhand_t * fptr, algr_t * algr, int fs, int as)
{
	name = _name;
	handle_type = fptr;
	algr_type = algr;
	fhand_size = fs;
	algr_size = as;
}
#ifndef __LINUX__
#define DLL_STAT	extern "C" __declspec(dllexport)
#else
#define DLL_STAT	extern "C"
#endif
#define MODULES_INFO   DLL_STAT modules_info __mod_info_load