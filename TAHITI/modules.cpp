#include "stdafx.h"
#include "modules.h"

long long mod_size=0;

Modules mod_poll[MAX_MOD_SIZE];

libDebug mod_fram("ModFrameWork");
bool framework_init = false;
void framework_inif()
{
	if (framework_init)
		return;
	mod_fram.setname("ModulesFramework");
	framework_init = true;
}


Modules::Modules()
{
	framework_inif();
};


string Modules::get_name()
{
	return sname;
}



void Modules::init()
{
	framework_inif();
	entry_client_ret = NULL;
	entry_client_to = NULL;
	entry_server = NULL;
	cmd_link = NULL;
}

void Modules::reg()
{
	mod_fram<< "reg of:" << this->sname << endl;
	mod_poll[mod_size] = *this;
	mod_size++;

}

Modules::Modules(feature_t _mode,string name, ESS_Modules __server_entry, ESS_CMOD __clinet_entry_to, ESS_CMOD __client_entry_ret,CLI kcli,KSAPI vlink)
{
	init();
	strcpy(sname, name.data());
	mode = _mode;
	cmd_link = kcli;
	entry_server = __server_entry;
	entry_client_ret = __client_entry_ret;
	entry_client_to = __clinet_entry_to;
	if (vlink != NULL)
		vlink();
	reg();
}

CLI Modules::get_cli()
{
	return cmd_link;
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

Modules::Modules(string name, CLI link, KSAPI vlink)
{
	init();
	mode = CLI_FEATURE;
	strcpy(sname, name.data());
	cmd_link = link;
	if (vlink != NULL)
		vlink();
	reg();
}