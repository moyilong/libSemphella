#pragma once
#include <libSemphella/main.h>
#define CONST_STR	"ELCP_EFU"
#define IMG_NAME_LEN	128

struct FF_HEAD {
	char EFU[sizeof(CONST_STR)] = CONST_STR;
	uint64_t file_len=-1;
	uint64_t build_ver=-1;
	char IMG_NAME[IMG_NAME_LEN];
	inline bool check_type()
	{
		if (strcmp(EFU, CONST_STR))
			return false;
		if (file_len == -1)
			return false;
		if (build_ver == -1)
			return false;
	}

};
