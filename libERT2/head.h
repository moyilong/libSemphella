#pragma once
#include "libERT2.h"
#define MAX_PATH_LEN	512
#define HEAD_VERIFY_LEN	8
#define HEAD_VERIFY_STR	"ERT2"


#pragma pack(push,1)
struct E2HEAD {
	char head[HEAD_VERIFY_LEN];
	int version;
	inline E2HEAD()
	{
		strcpy(head, HEAD_VERIFY_STR);
		version = ERT2_VER;
	}
};

struct E2FOOT {
	uint64_t filenumber;
	uint64_t password_hash;
	int algrid;
	bool crypt_filename = false;
	int block_size;
};

struct E2FAT {
	uint64_t fileid;
	char fname[MAX_PATH_LEN];
	uint64_t filelen;
	uint64_t filehash;
	uint64_t filebegin;
};
#pragma pack(pop)