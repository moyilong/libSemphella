#pragma once
#include "EMU_EEPROM.h"
struct PARTITION {
	//string name;
	char name[128];
	int id;
	uint64_t begin;
	uint64_t length;
};

struct FLASH {
	char name[128];
	int partition_size;
	PARTITION *part;
	uint64_t length;
	FLASH(const char * na,int ps, PARTITION *pt,uint64_t len);
	PARTITION GetPart(string name) const;
	PARTITION GetPart(int id)const ;
	inline FLASH() {};
};

#define KB	1024
#define MB	KB*1024
#define GB	MB*1024

#define DEVICE static FLASH 