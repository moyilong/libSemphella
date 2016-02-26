#pragma once
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>

uint64_t password_type(string passwd);



typedef void(*password_algrthom)(string password);	//初始化密码
typedef void(*crypt_algrthom)(char *data, int64_t len, int64_t bit_off);	//加密算法
typedef uint64_t(*sum_algrthom)(const char *data, int64_t len);	//校验和算法
typedef uint64_t(*get_password_checksum)(string passwd);	//密码校验算法

extern  bool decrypt;

class ALGHRTHIM {
public:
	password_algrthom pa;
	crypt_algrthom ca;
	int id;
	sum_algrthom sa;
	get_password_checksum px;
	inline ALGHRTHIM() {};
	ALGHRTHIM(password_algrthom p, crypt_algrthom c, sum_algrthom s, int xid, get_password_checksum pc=password_type);
};


#define ALGORTHIM static ALGHRTHIM
#define MAX_SIZE	8
//extern vector<ALGHRTHIM> APOLL;
extern ALGHRTHIM APOLL[MAX_SIZE];

extern int xsize;
int trans_id(int id);


