#include "stdafx.h"
#include "modules.h"

long long mod_size = 0;

Modules mod_poll[MAX_MOD_SIZE];

#define mod_fram if (false) cout<<"[ModFrameWork]"

bool framework_init = false;
void framework_inif()
{
	if (framework_init)
		return;
	mod_fram << "Fraemwork Init!" << endl;
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

void Modules::reg_client(ESS_CMOD x_client_to, ESS_CMOD x_client_ret)
{
	entry_client_ret = x_client_ret;
	entry_client_to = x_client_to;
}

void Modules::reg_cli(CLI xcli)
{
	cmd_link = xcli;
}

void Modules::reg_ser(ESS_Modules srv)
{
	entry_server = srv;
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
#ifdef UNLOAD_MODULES
	return;
#else
	mod_fram << "reg of:" << this->sname << endl;
	mod_poll[mod_size] = *this;
	mod_size++;
	ENV_DATA env;
	env.insert_modules = sname;
	env.insert_position = mod_size - 1;
	trigger(MODULES_LOAD, env);
#endif
}

Modules::Modules(feature_t _mode, string name, ESS_Modules __server_entry, ESS_CMOD __clinet_entry_to, ESS_CMOD __client_entry_ret, permission_t level, CLI kcli, KSAPI vlink)
{
	init();
	strcpy(sname, name.data());
	mode = _mode;
	cmd_link = kcli;
	entry_server = __server_entry;
	entry_client_ret = __client_entry_ret;
	entry_client_to = __clinet_entry_to;
	plevel = level;
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

Modules get_mod(uint64_t ID)
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

string cli_preprocess(vector<string>poll, string getd)
{
	for (int n = 0; n < poll.size(); n++)
	{
		if (poll.at(n).at(0) == '-')
		{
			int qeual = -1;
			for (int x = 0; x < poll.at(n).size(); x++)
				if (poll.at(n).at(x) == '=')
					qeual = x;
			if (qeual == -1)
			{
				return "y";
			}
			else {
				return poll.at(n).substr(qeual);
			}
		}
	}
	return "";
}

permission_t Modules::get_level()
{
	return plevel;
}

int modules_info(vector<string>)
{
	for (int n = 0; n < mod_size; n++)
	{
		cout << "[" << n << "]" << mod_poll[n].get_name() << "::";
		if (mod_poll[n].get_cli() != NULL)
			cout << "c";
		if (mod_poll[n].get_client_ret() != NULL)
			cout << 'r';
		if (mod_poll[n].get_client_to() != NULL)
			cout << "s";
		if (mod_poll[n].get_server_entry() != NULL)
			cout << "X";
		cout << endl;
	}
	return 0;
}

Modules __modules_info("ModulesInfo", modules_info);