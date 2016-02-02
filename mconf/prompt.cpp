#include "stdafx.h"
#include "loaddef.h"

vector<string>str;

void build_block(BLOCK_INFO blk)
{
	string config_line = "config " + blk.name;
	str.push_back(config_line);
	string type_line = "\t" + blk.type;
	str.push_back(type_line);
	string prompt_line = "\tprompt \"" + blk.prompt + "\"";
	str.push_back(prompt_line);
	if (!blk.depends.empty())
	{
		string depends = "\tdepends on " + blk.depends;
		str.push_back(depends);
	}
	if (!blk.default_val.empty())
	{
		string default_line = "\tdefault \"" + blk.default_val + "\"";
		str.push_back(default_line);
	}
	str.push_back("");
}

void build_menu(MENU &menu)
{
	str.push_back("menu");
	string menubar = "prompt \"" + menu.display_name + "\"\n";
	str.push_back(menubar);
	cout << "Menu " << menu.display_name << " msize=" << menu.mdata.size() << " bsize=" << menu.data.size() << endl;
	for (long long n = 0; n < menu.mdata.size();n++)
	{
		build_menu(menu.mdata.at(n));
	}
	for (long long n = 0; n < menu.data.size(); n++)
		build_block(menu.data.at(n));
	str.push_back("endmenu");
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

vector<string> dot_string;

void read_block(BLOCK_INFO blk)
{
	dot_string.push_back(get_config(blk.uci_info));
}

void read_config(MENU &menu)
{
	string menu_start = "# Menu Configure of: " + menu.display_name;
	dot_string.push_back(menu_start);
	for (uint64_t n = 0; n < menu.mdata.size();n++)
		read_config(menu.mdata.at(n));
	for (uint64_t n = 0; n < menu.data.size(); n++)
		read_block(menu.data.at(n));
	string menu_end = "# End Menu Configure of: " + menu.display_name;
	dot_string.push_back(menu_end);
}

void Prompt_DotConfig(string filename)
{
	ofstream out;
	out.open(filename.data());
	read_config(main_menu);
	for (uint64_t n = 0; n < dot_string.size(); n++)
		out << dot_string.at(n) << endl;
	out.flush();
	out.close();
}