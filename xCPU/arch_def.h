#pragma once
#include <libSemphella/libSemphella.h>
//#define ASSIGN_TYPE	uint64_t
typedef uint64_t ASSIGN_TYPE;
#define SIGNED_TYPE	int64_t
#define ARCH_LENGTH	sizeof(ASSIGN_TYPE)

#define ATYPE_MAX	numeric_limits<ASSIGN_TYPE>::max()
#define ATYPE_MIN	numeric_limits<ASSIGN_TYPE>::min()

#define SECURE_ZONE_SIZE	128
#define ALLOW_SIZE	128*MB
#define MEM_SIZE	ALLOW_SIZE-SECURE_ZONE_SIZE
#define BOOL_VAL_AREA	128