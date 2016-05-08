#pragma once
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>
#include "libERT.h"

uint64_t password_type(string passwd);

typedef void(*password_algrthom)(string password);	//初始化密码
typedef void(*crypt_algrthom)(char *data, int64_t len, int64_t bit_off);	//加密算法
typedef uint64_t(*sum_algrthom)(const char *data, int64_t len);	//校验和算法
typedef uint64_t(*get_password_checksum)(string passwd);	//密码校验算法

class ALGHRTHIM {
public:
	bool can_be_pt = true;
	//密码初始化算法
	password_algrthom pa;
	//加密算法
	crypt_algrthom ca;
	int id;
	//校验值算法
	sum_algrthom sa;
	//密码校验算法
	get_password_checksum px;
	//说明
	string doc;
	inline ALGHRTHIM() {};
	ALGHRTHIM(password_algrthom p, crypt_algrthom c, sum_algrthom s, int xid, string _doc, get_password_checksum pc = password_type, bool _can_be_pt = true);
};
#define AMAX	16

#define ALGORTHIM static ALGHRTHIM
//extern vector<ALGHRTHIM> APOLL;
extern ALGHRTHIM APOLL[AMAX];

extern int xsize;
int trans_id(int id);
