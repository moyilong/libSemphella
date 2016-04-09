#pragma once
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>
#include "cryptutil2.h"

uint64_t password_type(string passwd);

typedef void(*password_algrthom)(string password);	//��ʼ������
typedef void(*crypt_algrthom)(char *data, int64_t len, int64_t bit_off);	//�����㷨
typedef uint64_t(*sum_algrthom)(const char *data, int64_t len);	//У����㷨
typedef uint64_t(*get_password_checksum)(string passwd);	//����У���㷨

extern  bool decrypt;

class ALGHRTHIM {
public:
	typedef void(*proc)(ALGHRTHIM *self);			//��ʼ������
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
	inline ALGHRTHIM() {};
	ALGHRTHIM(password_algrthom p, crypt_algrthom c, sum_algrthom s, int xid, get_password_checksum pc = password_type, bool _can_be_pt = true);
	ALGHRTHIM(proc initfunc);
};
#define AMAX	16

#define ALGORTHIM static ALGHRTHIM
//extern vector<ALGHRTHIM> APOLL;
extern ALGHRTHIM APOLL[AMAX];

extern int xsize;
int trans_id(int id);
