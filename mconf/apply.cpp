#include "stdafx.h"
#include "loaddef.h"
#define NOT_SET	" is not set"
vector<CONFIG> confpoll;

CONFIG search(string name)
{
	long long uid = -1;
#pragma omp parallel for
	for (long long n = 0; n < confpoll.size(); n++)
	{
		if (confpoll.at(n).name == name)
			uid = n;
	}
	if (uid == -1)
		return CONFIG();
	return confpoll.at(uid);
}

void apply_block(BLOCK_INFO blk)
{
	CONFIG cfg = search(blk.name);
	if (!(blk.default_val == cfg.data))
		set_config(blk.uci_info, cfg.data);
}

void apply_menu(MENU &menu)
{
	for (long long n = 0; n < menu.mdata.size(); n++)
		apply_menu(menu.mdata.at(n));
	for (long long n = 0; n < menu.data.size(); n++)
		apply_block(menu.data.at(n));
}

void Apply(string input)
{
	ifstream in;
	in.open(input.data());
	if (!in.is_open())
		return;
	while (!in.eof())
	{
		string temp;
		getline(in, temp);
		CONFIG xtemp;
		if (temp.at(0) == '#' && streval(temp.substr(0 - strlen(NOT_SET)).data(), NOT_SET))
		{
			xtemp.name = temp.substr(2, temp.size() - 2 - strlen(NOT_SET));
			xtemp.data = "0";
		}
		else {
			if (temp.at(0) == '#')
				continue;
			else
			{
				int eq = strfind(temp.data(), '=', true);
				xtemp.name = temp.substr(0, eq);
				xtemp.data = temp.substr(eq + 1);
				if (xtemp.data == "y")
					xtemp.data = "1";
			}
		}
		xtemp.name = xtemp.name.substr(5);
		confpoll.push_back(xtemp);
	}
	apply_menu(main_menu);
}