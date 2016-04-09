#pragma once
#define ASSIGN_TYPE	uint64_t
#define SIGNED_TYPE	int64_t
#define ARCH_LENGTH	sizeof(ASSIGN_TYPE)

#define ATYPE_MAX	numeric_limits<ASSIGN_TYPE>::max()
#define ATYPE_MIN	numeric_limits<ASSIGN_TYPE>::min()


#define KB	1024
#define MB	KB*1024
#define GB	MB*1024

#define SECURE_ZONE_SIZE	128
#define ALLOW_SIZE	8*MB
#define MEM_SIZE	ALLOW_SIZE-SECURE_ZONE_SIZE