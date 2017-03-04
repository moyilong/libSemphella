#pragma once
#include <assert.h>
#include <errno.h>

enum ERRNO {
	INVALID_ARGMENTS,
	INVALID_DICT_FIND,
};

extern API const char *error_list[];