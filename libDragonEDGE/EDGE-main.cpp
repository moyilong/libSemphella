#include "libDragonEDGE.h"

vector<modules>poll;

modules::modules(long long xcall, mod_entry api)
{
	call = xcall;
	ent = api;
	poll.push_back(*this);
}

modules::modules(long long xcall, mod_entry api, void_type xloop)
{
	loop = xloop;
	modules(xcall, api);
}

void call_modules(IO_BUFF &io, IO_BUFF get)
{
	for (int n = 0; n < poll.size();n++)
		if (poll.at(n).call == get.cmdcall)
		{
			poll.at(n).ent(io, get);
			return;
		}
	io.cmdcall = PACKAGE_UNDEF;
}

vector<modules> &getpoll()
{
	return poll;
}