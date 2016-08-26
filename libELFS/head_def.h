#pragma once
#include <inttypes.h>
#define STRING_LEN	128
#define ATTR_SIZE	32
#define API_FS		0xFF003F5D
#define FS_VER		10.1

enum MATRIX_TYPE {
	HYPER_BOOST,		//RAID0
	READ_BOOST,			//RAID1
	MIXED_BOOST,		//RAID5
};

#pragma pack(push,1)

struct attribute {
	char name[STRING_LEN];
	char val[STRING_LEN];
};

struct efs_head {
	int dhead = API_FS;
	attribute attr[ATTR_SIZE];
	int ver = FS_VER;
	uint64_t begin_offset;
	uint64_t block_size;
	uint64_t block_len;
	//Configure of Matrix
	bool in_matrix;
	uint64_t matrix_id;
	uint64_t in_matrix_id;
	MATRIX_TYPE matrix;
};


#pragma pack(pop)

