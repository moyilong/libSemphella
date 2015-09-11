#pragma once

#define FEATURE_API	0x5D00
#define COMPACT_API	0x5DFF
#define DEFAULT_PASSWORD	"moyilong_Project_Insight"
#define DEFAULT_PORTS	9999

#define DEVICE_SERIAL_TYPE	unsigned long long;

struct CONN_HEAD {
	char F_API = FEATURE_API;
	char C_API = COMPACT_API;
	long long type;
	int protoco=0;

};

#define HEAD_LEN	sizeof(CONN_HEAD)

#define APICALL_NOTHING	0xFFFFFF01
#define APICALL_ERROR	0xFFFFFF02

/*
*
*
*	Basic Feature Level
*
*
*/

#define SWITCH_PROTOCO		0xFFFF0000
#define ENABLE_FAILD_SAFE	0xFFFF0001
#define DISABLE_FAILD_SAFE	0xFFFF0002

