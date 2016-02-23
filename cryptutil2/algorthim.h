#pragma once
#include <libSemphella/main.h>
#include <libSemphella/files.h>
#include <libSemphella/utils.h>
#include <libSemphella/crypt.h>
#include <libSemphella/debug.h>

typedef void(*password_algrthom)(string password);
typedef void(*crypt_algrthom)(char *data, int64_t len, int64_t bit_off);
typedef uint64_t(*sum_algrthom)(const char *data, int64_t len);


extern  bool decrypt;

struct ALGHRTHIM {
	password_algrthom pa;
	crypt_algrthom ca;
	int id;
	sum_algrthom sa;
	ALGHRTHIM::ALGHRTHIM(password_algrthom p, crypt_algrthom c, sum_algrthom s, int xid);
};


#define ALGORTHIM static ALGHRTHIM

extern vector<ALGHRTHIM> APOLL;

int trans_id(int id);

