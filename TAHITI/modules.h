#pragma once
#define MAX_MOD_SIZE	MAX_BUFF_SIZE
#include "linux_support.hpp"
#include "feature_define.h"

typedef void(*ESS_Modules)(struct DATA_FORMAT,struct DATA_FORMAT &,SOCKET &);
typedef void(*ESS_CMOD)(struct DATA_FORMAT &);
void ErrSetBack(struct DATA_FORMAT &ret, const char *ERR_INFO);

class Modules{
public:
	Modules();
	Modules(feature_t _mode,string name, ESS_Modules __server_entry = NULL, ESS_CMOD __clinet_entry_to = NULL, ESS_CMOD __client_entry_ret = NULL);
	ESS_Modules get_server_entry();
	ESS_CMOD get_client_to();
	ESS_CMOD get_client_ret();
	feature_t get_api();
	string get_name();
private:
	int mode;
	ESS_CMOD entry_client_to;
	ESS_CMOD entry_client_ret;
	ESS_Modules entry_server;
	char sname[MAX_BUFF_SIZE];
};

Modules get_mod(unsigned long long ID);
int get_modules_size();