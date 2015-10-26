#pragma once
#define API_VERSION	0xF0
#define TABLE_LEN	32
#define DSDT_LEN	128
#define CHECK_STRING	"ELONE-DragonOS DSDT Table"

#ifndef VENDOR
#define VENDOR	0x59DD
#endif

#ifndef PRODUCT
#define PRODUCT	0x9999
#endif

#include <time.h>

#ifndef OS_TYPE
#define OS_TYPE	"unknow"
#endif


enum BIOS_STATUS{
	NORMALLY,
	CHECK_FAILD,
	OPEN_FAID,
};

struct MONITOR_TABLE{
	unsigned long long power_up_time;
	unsigned long long power_up_count;
	time_t first_init_time=-1;
};

struct BIOS_INFO{
	char check_area[sizeof(CHECK_STRING)];
	char version_info[256];
	char api_ver = API_VERSION;
	bool bool_table[TABLE_LEN];
	int dsdt[DSDT_LEN];
	unsigned long long vendor = VENDOR;
	unsigned long long product = PRODUCT;
	bool write_skip = false;
	MONITOR_TABLE EFI;
	char OS[32];
};


class CAPI BIOS{
public:
	BIOS(string file,string sign,bool create=false);
	BIOS(string sign,bool create=false,string file=getenv("BIOS_FILE"));
	~BIOS();
	enum BIOS_STATUS last_stat = NORMALLY;
	void setenv(int addr, char value);
	int readenv(int addr);
	void write();
	struct BIOS_INFO info;
private:
	fstream iobit;
	string passwd;
protected:
};