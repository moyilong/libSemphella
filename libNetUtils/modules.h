#pragma once
#include "protoco.h"

typedef void(*server_api)(SOCKET sock, void *get_data, uint64_t get_len, void *send_ptr, uint64_t &send_len);

struct modules {
	WORK_CMD cmd;
	server_api api;
	modules(WORK_CMD _cmd, server_api _api);
};

#define MOD	static modules

extern vector<modules> modpoll;

#define API_ARGS_DEF	SOCKET sock, void *get_data, uint64_t get_len, void *send_ptr, uint64_t &send_len
SOCKET create_connect_server(int port);
SOCKET create_connect(int port, string target);