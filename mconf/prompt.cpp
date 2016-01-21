#include "stdafx.h"
#include "loaddef.h"

vector<string>str;

void build_block(BLOCK_INFO blk)
{
	string config_line = "config " + blk.name;
	str.push_back(config_line);
	string prompt_line = "\t" + blk.type+" \"" + blk.prompt + "\n";
	str.push_back(prompt_line);
	if (!blk.depends.empty())
	{
		string depends = "\tdepends on " + blk.depends;
		str.push_back(depends);
	}
}

void build_menu(MENU &menu)
{
	str.push_back("menu");
	string menubar = "prompt \"" + menu.display_name + "\"";
	str.push_back(menubar);
	for (long long n = 0; n < menu.mdata.size();n++)
	{
		build_menu(menu.mdata.at(n));
	}
	str.push_back("endmenu");
	for (long long n = 0; n < menu.data.size(); n++)
		build_block(menu.data.at(n));
}

void Prompt(string filename)
{
	cout << "GEN\t" << filename << endl;
	str.clear();
	ofstream out;
	out.open(filename.data());
	build_menu(main_menu);
	out << "#\n#\n# This Documents is Create by DragonOS mconf utils\n# Do not Edit !\n#\n#\n\n\n";
	for (unsigned long long  n = 0; n < str.size(); n++)
		out << str.at(n) << endl;
}