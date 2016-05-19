#pragma once
#include "libERT.h"
#include "external_library.h"
uint64_t password_type(string passwd);

class ALGHRTHIM {
public:
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
	inline ALGHRTHIM() {};
	ALGHRTHIM(password_algrthom p, crypt_algrthom c, sum_algrthom s, int xid, string _doc, get_password_checksum pc = password_type, bool _can_be_pt = true);
	ALGHRTHIM(algr_t algr);
};
#define AMAX	16

#define ALGORTHIM static ALGHRTHIM
//extern vector<ALGHRTHIM> APOLL;
extern ALGHRTHIM APOLL[AMAX];

extern int xsize;
int trans_id(int id);
extern bool decryptmode;