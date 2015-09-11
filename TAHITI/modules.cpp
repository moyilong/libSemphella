#include "stdafx.h"
#include "modules.h"
vector<Modules>poll;


Modules::Modules()
{
};


Modules::Modules(feature_t _mode,string name, ESS_Modules __server_entry, ESS_CMOD __clinet_entry_to, ESS_CMOD __client_entry_ret)
{
	inited = true;
	sname = name;
	mode = _mode;
	entry_server = __server_entry;
	entry_client_ret = __client_entry_ret;
	entry_client_to = __clinet_entry_to;	
	poll.push_back(*this);
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

unsigned long long get_modules_size()
{
	return poll.size();
}

Modules get_mod(unsigned long long ID)
{
	return poll.at(ID);
}

feature_t Modules::get_api()
{
	return mode;
}