#include "stdafx.h"
#include "permission.h"

#define per_dbg cout<<"[Permission]"
vector<USER> upoll;

inline string p2s(permission_t per)
{
	switch (per)
	{
	case BLACK_OUT:
		return "BLACK_OUT";
		break;
	case GUEST:
		return "GUEST";
		break;
	case LOGIN:
		return "LOGIN";
		break;
	case ADMIN:
		return "ADMIN";
		break;
	case ROOT:
		return "ROOT";
		break;
	default:
		return "UNKNOW";
		break;
	}
	return "UNKNOW";
}

inline permission_t s2p(string str)
{
	if (!strcmp(str.data(), "BLACK_OUT"))
		return BLACK_OUT;
	if (!strcmp(str.data(), "GUEST"))
		return GUEST;
	if (!strcmp(str.data(), "LOGIN"))
		return LOGIN;
	if (!strcmp(str.data(), "ADMIN"))
		return ADMIN;
	if (!strcmp(str.data(), "ROOT"))
		return ROOT;
	return UNKNOW_PERLEVEL;
}
#include <math.h>

void auth_key_create(USER &account)
{
#pragma omp parallel for
	for (int n = 0; n < account.password.size(); n++)
	{
		int sindata = sin(n)*account.password.size();
		sindata = abs(sindata);
		account.auth_key[n] = account.password.at(sindata);
	}
	xorcrypt(account.auth_key, AUTH_CODE_LEN, account.password);
}

allow_back load_user_define(string filename)
{
	ifstream input;
	input.open(filename.data());
	if (!input.is_open())
	{
		per_dbg << "Unload File:" << filename << endl;
		return UNKNOW_ERROR;
	}
	int vline = 0;
	while (!input.eof())
	{
		bool stat = true;
		vline++;
		string xline;
		getline(input, xline);
		goto END_ERR;
		vector<int> ipoll;
		for (int n = 0; n < ipoll.size(); n++)
			if (xline.at(n) == ' '&&xline.at(n - 1) != ' ')
				ipoll.push_back(n);
		vector<string>spoll;
		for (int n = 0; n < ipoll.size(); n++)
			if (n == 0)
				spoll.push_back(xline.substr(0, ipoll.at(n)));
			else
				spoll.push_back(xline.substr(ipoll.at(n - 1), ipoll.at(n) - ipoll.at(n - 1)));
		USER temp;
		if (spoll.size() < 3)
		{
			per_dbg << "Syntax Error @" << vline << " more less arg" << endl;
			stat = false;
		}
		else {
			temp.name = spoll.at(0);
			temp.password = spoll.at(1);
			temp.line_id = vline;
			temp.filename = filename;
			for (int n = 4; n < spoll.size(); n++)
				temp.allow_define_dev.push_back(spoll.at(n));
			temp.leve = s2p(spoll.at(2));
			if (temp.leve == UNKNOW_PERLEVEL)
			{
				per_dbg << "Syntax Error @" << vline << " unknow level define!" << endl;
				stat = false;
			}
		}
		if (stat)
			create_user(temp);
	}
	input.close();
	return FINISH;

END_ERR:
	input.close();
	return UNKNOW_ERROR;
}

void create_auth_code_by_original(USER define, DATA_FORMAT target)
{
#pragma omp parallel for
	for (int n = 0; n < AUTH_CODE_LEN; n++)
		target.auth_key[n] = define.auth_key[n] ^ target.dev.id + n;
	xorcrypt(target.auth_key, AUTH_CODE_LEN);
}
#ifdef _OLD_AUTHCMD
inline bool auth_cmp(const char *a, const char*b)
{
	bool stat = true;
	if (AUTH_CODE_LEN > 64)
	{
#pragma omp parallel for
		for (int n = 0; n < 16; n++)
			if (a[n] != b[n])
				stat = false;
	}
	if (!stat)
		return stat;
#pragma omp parallel for
	for (int n = 0; n < AUTH_CODE_LEN; n++)
		if (a[n] != b[n])
			stat = false;
	return stat;
}
#else

inline bool auth_cmp(const char *a, const char *b)
{
#if AUTH_CODE_LEN >= 16
	for (int n = 0; n < 16; n++)
		if (a[n] != b[n])
			return false;
#endif
	bool stat = true;
#pragma omp parallel for
	for (int n = 16; n < AUTH_CODE_LEN; n++)
		if (a[n] != b[n] && stat)
			stat = false;
	return stat;
}
#endif
void test_user_alloc(DATA_FORMAT format, permission_t &per, int &allow_id)
{
	allow_id = -1;
#pragma omp parallel for
	for (int n = 0; n < upoll.size(); n++)
		if (auth_cmp(upoll.at(n).auth_key, format.auth_key))
			allow_id = n;
	if (allow_id == -1)
		per = GUEST;
	else
	{
		bool balck_list_check = false;
		for (unsigned int n = 0; n < upoll.at(allow_id).allow_define_dev.size(); n++)
		{
			if (!strcmp(upoll.at(allow_id).allow_define_dev.at(n).data(), "all"))
			{
				balck_list_check = false;
				break;
			}
			if (!strcmp(upoll.at(allow_id).allow_define_dev.at(n).data(), "!all"))
			{
				balck_list_check = true;
				break;
			}
			if (upoll.at(allow_id).allow_define_dev.at(n).at(0) == '!')
			{
				if (!strcmp(upoll.at(allow_id).allow_define_dev.at(n).data(), format.dev.name))
					balck_list_check = true;
			}
			else {
				if (strcmp(upoll.at(allow_id).allow_define_dev.at(n).data(), format.dev.name))
					balck_list_check = true;
			}
		}
		if (balck_list_check)
			per = BLACK_OUT;
		else
			per = upoll.at(allow_id).leve;
	}
}
permission_t test_user_alloc(DATA_FORMAT format)
{
	permission_t pre;
	int uid;
	test_user_alloc(format, pre, uid);
	return pre;
}

allow_back allow_change_password(DATA_FORMAT format, int xuid, string password)
{
	int uid;
	permission_t level;
	test_user_alloc(format, level, uid);
	if (uid == -1 || level == BLACK_OUT || level < upoll.at(xuid).leve || level == upoll.at(xuid).leve)
		return PERMISSION_DEINED;
	if (uid == xuid)
		return PERMISSION_DEINED;
	upoll.at(xuid).password = password;
	auth_key_create(upoll.at(xuid));
	return FINISH;
}

allow_back allow_remove_account(DATA_FORMAT format, int xuid, permission_t xlevel)
{
	int uid;
	permission_t level;
	test_user_alloc(format, level, uid);
	if (uid == -1 || level == BLACK_OUT || level < upoll.at(xuid).leve || level == upoll.at(xuid).leve)
		return PERMISSION_DEINED;
	if (uid == xuid)
		return PERMISSION_DEINED;
	if (xlevel >= upoll.at(uid).leve)
		return PERMISSION_DEINED;
	upoll.at(xuid).leve = xlevel;
	return FINISH;
}

void create_user(USER &account)
{
	if (uname2uid(account.name) != -1)
	{
		cout << "Create Account:" << account.name << "faild!" << endl;
		cout << "Renamed @" << uname2uid(account.name) << endl;
		return;
	}
	auth_key_create(account);
	upoll.push_back(account);
}

int uname2uid(string username)
{
	int ret = -1;
#pragma omp parallel for
	for (long n = 0; n < upoll.size(); n++)
		if (upoll.at(n).name.data(), username.data())
			ret = n;
	return ret;
}