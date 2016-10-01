#include "modules.h"

vector<_CAT_MOD> vmod;

_CAT_MOD::_CAT_MOD(string name, CAT_API api,  CAT_PROC init_call, CAT_PROC endcall)
{
	dname = name;
	dapi = api;
	dinit_call = init_call;
	dendcall = endcall;
	vmod.push_back(*this);
}

_CAT_MOD Search(string name)
{
	for (int n = 0; n < vmod.size(); n++)
		if (streval(name.data(), vmod.at(n).dname.data()))
			return vmod.at(n);
	throw "NO_ENOUGTH_VMOD";
}
