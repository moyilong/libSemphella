#pragma once
#include "modules.h"
void server_main(ESS_Modules api_entry = NULL, permission_t min_call_level = BLACK_OUT);
void client_main();
#include "feature_define.h"
#include "ExtenisionLIB/configure.h"
struct XKERNEL : kernel{
	string server = "localhost";
	int port = PORT;
	string device_name="unknow_test";
	string confdir;
	int max_try_conn=300;
	int try_wait = 500;
	int mod_call_wait = 1500;
	bool quiet = false;
	bool old_protoco_version = true;
	bool allow_server_back_level = false;
	uint64_t deviId = 0;
	string kernel_mode;
	XKERNEL();
};
//KERNEL &kernel();
extern XKERNEL kernel;

#define kernel() kernel
