// EV_APP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/main.h>
#include <libSemphella/sysapi_modcall.h>

#include <libSemphella/argment.h>
#include "CALL_TYPE.h"
string mod_api;
argment targ;

enum WMODE {
	APP_MAIN_RUN,
	APP_INFO_GET,
};
WMODE mode = APP_MAIN_RUN;
void load_args(string name, string val)
{
	switch (name.at(0))
	{
	case 'l':
		switch (name.at(1))
		{
		case 'n':
			mod_api = val;
			break;
		case 'i':
			mode = APP_INFO_GET;
			break;
		}
		break;
	default:
		targ.add(name, val);
		break;
	}
}
int main(int argc, char *argv[])
{
	argment arg;
	arg.load(argc, argv);
	arg.for_each(load_args);
	if (mod_api.empty())
	{
		cout << "Warring:Argments Error!" << endl;
		return -1;
	}
	mod_t call = OpenLibrary(mod_api.data());
	if (call == NULL)
	{
		cout << "Open Application Faild!" << endl;
		return -1;
	}
	APP_ENTRY *entry = (APP_ENTRY*)GetLibraryAddress(call, "__main_entry_info");
	if (entry == NULL)
	{
		cout << "No Entry Found!" << endl;
		return -1;
	}
	if (mode == APP_MAIN_RUN)
	{
		debug << "Modules Was been init:" << entry->appname << "," << entry->ver.to_str() << endl;
		if (entry->init_call != NULL)
			entry->init_call();
		int ret = 0;
		if (entry->main_ent != NULL)
		{
			try {
				ret = entry->main_ent(targ);
			}
			catch (...)
			{
				ret = -1;
				cout << "Modules Return an Error!" << endl;
			}
		}
		if (entry->end_call != NULL)
			entry->end_call();
		return ret;
	}
	else if (mode == APP_INFO_GET)
	{
		cout << "====================================" << endl;
		cout << "Name:" << entry->appname << endl;
		cout << "Version:" << entry->ver.to_str() << endl;
		cout << "Program API:" << entry->pg_ver.to_str() << endl;
		CloseLibrary(call);
		return 0;
	}
}