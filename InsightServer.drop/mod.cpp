#include "feature.h"
#include "mod.h"

#include <libSemphella/debug.h>

libDebug mod_api("ModulesFramework");

struct STORAGE_FORMAT{
	modules_api api;
	long long call_type;
};

vector<STORAGE_FORMAT> poll;

mod::mod(long long call_type, modules_api api)
{
	STORAGE_FORMAT info;
	info.call_type = call_type;
	info.api = api;
	poll.push_back(info);
}

modules_api get_modapi(long long reg_type)
{
	for (int n = 0; n < poll.size(); n++)
		if (poll.at(n).call_type == reg_type)
			return poll.at(n).api;
	return NULL;
}