#include "stdafx.h"
#include "loaddef.h"
#ifndef __LINUX
#include "local_uci_head/uci.h"
#else
#include <uci.h>
#endif

string get_config(string uci)
{
#ifndef __LINUX
	return "only_test_for_windows!";
#else
	string ret;
	uci_context *context = uci_alloc_context();
	char g = 0;
	int dot[4];
	for (int n = 0; n < uci.size(); n++)
		if (uci[n] == '.')
			dot[g++] = n;
	string file = uci.substr(0, dot[0]);
	string package = uci.substr(dot[0] + 1, dot[1]);
	string section = uci.substr(dot[1] + 1, dot[2]);
	string option = uci.substr(dot[3] + 1);
	uci_package *pkg;
	if (UCI_OK != uci_load(context, file.data(), &pkg))
	{
		DEBUG << "Luci Open FIle Faild!" << endl;
		exit(-1);
	}
	uci_element *X;
	uci_foreach_element(&pkg->sections, X)
	{
		uci_section *s = uci_to_section(X);
		ret = uci_lookup_option_string(context, s, option.data());
	}
	return ret;
#endif
}

string set_config(string uci, string value)
{
#ifndef __LINUX
	DEBUG << "Setting Configure:" << uci << "=" << value << endl;
	return"";
#else
	uci_context *context = uci_alloc_context();
	char g = 0;
	int dot[4];
	for (int n = 0; n < uci.size(); n++)
		if (uci[n] == '.')
			dot[g++] = n;
	string file = uci.substr(0, dot[0]);
	string package = uci.substr(dot[0] + 1, dot[1]);
	string section = uci.substr(dot[1] + 1, dot[2]);
	string option = uci.substr(dot[3] + 1);
	struct uci_ptr prt;
	prt.package = package.data();
	prt.section = section.data();
	prt.option = option.data();
	prt.value = value.data();
	uci_set(context, &prt);
	uci_commit(context, &prt.p, true);
	uci_unload(context, prt.p);
	uci_free_context(context);
#endif
}