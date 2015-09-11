#include "stdafx.h"
#include "modules.h"

long long mod_size=0;

Modules mod_poll[MAX_MOD_SIZE];


Modules::Modules()
{
};


string Modules::get_name()
{
	return sname;
}

Modules::Modules(feature_t _mode,string name, ESS_Modules __server_entry, ESS_CMOD __clinet_entry_to, ESS_CMOD __client_entry_ret)
{
	DEBUG_LINE cout << "reg of:" << name << endl;
	strcpy(sname, name.data());
	mode = _mode;
	entry_server = __server_entry;
	entry_client_ret = __client_entry_ret;
	entry_client_to = __clinet_entry_to;
	mod_poll[mod_size] = *this;
	mod_size++;
}

ESS_Modules Modules::get_server_entry()
{
	return entry_server;
}
ESS_CMOD Modules::get_client_to()
{
	return entry_client_to;
}

ESS_CMOD Modules::get_client_ret()
{
	return entry_client_ret;
}

int get_modules_size()
{
	return mod_size;
}

Modules get_mod(unsigned long long ID)
{
	return mod_poll[ID];
}

feature_t Modules::get_api()
{
	return mode;
}