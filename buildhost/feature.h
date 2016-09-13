#pragma once
#include <libSemphella/string.h>
#include <libSemphella/debug.h>
#include <libSemphella/files.h>

#include <vector>
using namespace std;

#define NAME_LEN	128
#define UID_LEN		128
#define MESSAGE_LEN 256

#pragma pack(push,2)
struct build_db {
	uint64_t build_id;
	char uid[UID_LEN];
};

struct build_log {
	char uid[UID_LEN];
	char project[NAME_LEN];
	char message[MESSAGE_LEN];
	uint64_t id;
	time_t date;
};
#pragma pack(pop)


class db {
public:
	db(string filename);
	inline db() {};
	void load(string filename);
	int log_keep_size = 64;
	build_log add_log(char project[NAME_LEN], char message[MESSAGE_LEN]);
private:
	build_db data;
	vector<build_log>logdb;
	file log_file;
	file main_file;
	void save_main_data();
	uint64_t add_log(build_log logs);
};

void CreateUID(char id[UID_LEN]);
extern db md;

#define _NETWORK_TRANSFAR_LEN	NAME_LEN+MESSAGE_LEN+UID_LEN
#if _NEWORK_TRANSFAR_LEN < 4096
#define NETWORK_TRANSFAR_LEN 4096
#else
//#define NETWORK_TRANSFAR_LEN	_NETWORK_TRANSFAR_LEN
#error Network Transt Buffers is too small
#endif

#pragma pack(push,1)
struct request {
	char project_name[NAME_LEN];
	char message[MESSAGE_LEN];
};
struct processed {
	uint64_t id;
	char uid[UID_LEN];
};
#pragma pack(pop)