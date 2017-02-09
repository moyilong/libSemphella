#include "stdafx.h"
#include "feature.h"

vector<_feature> cmd_poll;

_feature::_feature(ASSIGN_TYPE cmd_reg, string reg_name, API_CALL call)
{
	reg = cmd_reg;
	name = reg_name;
	api = call;
	cmd_poll.push_back(*this);
}

void exec(string reg_name)
{
	for (int n = 0; n < cmd_poll.size(); n++)
		if (streval(reg_name.data(), cmd_poll.at(n).name.data()))
			cmd_poll.at(n).api();
}

void exec(ASSIGN_TYPE type)
{
	for (ASSIGN_TYPE n = 0; n < cmd_poll.size(); n++)
		if (cmd_poll.at(n).reg == type)
			cmd_poll.at(n).api();
}

ASSIGN_TYPE search(const string name)
{
	for (ASSIGN_TYPE n = 0; n < cmd_poll.size(); n++)
		if (cmd_poll.at(n).name == name)
			return n;
	return 0;
}