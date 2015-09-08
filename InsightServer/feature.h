#pragma once

#define FEATURE_API	0x5D00
#define COMPACT_API	0x5DFF
#define DEFAULT_PASSWORD	"moyilong_Project_Insight"
#define DEFAULT_PORTS	9999

#define DEVICE_SERIAL_TYPE	unsigned long long;

enum WORK_TYPE {
	NOTHING,
};


struct CONN_HEAD {
	char F_API = FEATURE_API;
	char C_API = COMPACT_API;
	WORK_TYPE type;
	int protoco=0;
};

#define HEAD_LEN	sizeof(CONN_HEAD)

