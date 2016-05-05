#pragma once
enum permission_t{
	UNKNOW_PERLEVEL=-2,
	BLACK_OUT=-1,
	GUEST=0x0,
	LOGIN=0x1,
	ADMIN=0x2,
	ROOT=0xFFFF,	
};

enum allow_back{
	FINISH,
	PERMISSION_DEINED,
	UNKNOW_ERROR
};

struct USER{
	string name;
	char auth_key[AUTH_CODE_LEN];
	string password;
	int line_id = 0;
	string filename;
	vector<string>allow_define_dev;
	permission_t leve;
	inline USER()
	{
		memset(auth_key, 0, sizeof(auth_key));
	}
};

allow_back load_user_define(string filename);
permission_t test_user_alloc(DATA_FORMAT format);
allow_back allow_change_password(DATA_FORMAT format, int xuid, string password);
allow_back allow_remove_account(DATA_FORMAT format,int xuid,permission_t level);
void auth_key_create(USER &account);
void create_user(USER &account);
int uname2uid(string username);
//username password level <allow_device> !<now_allow_device>