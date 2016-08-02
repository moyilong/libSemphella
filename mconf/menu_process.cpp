#include "stdafx.h"
#include "loaddef.h"

MENU* GetProcAddr(string section, MENU &search_from)
{
	DEBUG << "Search " << section << " from " << search_from.section << endl;
	if (streval(section.data(), "main"))
	{
		DEBUG << "direct ret main" << endl;
		return &main_menu;
	}
	if (streval(section.data(), search_from.section.data()))
		return &search_from;
	MENU *ret = EMPTY_RET;
#pragma omp parallel for
	for (long long n = 0; n < search_from.mdata.size(); n++)
	{
		MENU *temp = GetProcAddr(section, search_from);
		if (temp != EMPTY_RET)
			ret = temp;
	}
	return ret;
}

MENU* CreateMenu(string section, string display, string father)
{
	if (streval(father.data(), "main"))
	{
		DEBUG << "main menu is direct ret" << endl;
		return &main_menu;
	}
	MENU *getd = GetProcAddr(section, main_menu);
	if (getd != EMPTY_RET)
		return getd;
	DEBUG << "Menu is not exist! Creteing!" << endl;
	getd = GetProcAddr(father, main_menu);
	if (getd == EMPTY_RET)
	{
		DEBUG << "Faild Create Menu form : " << father << endl;
		exit(-1);
	}
	MENU pushd;
	pushd.section = section;
	pushd.display_name = display;
	getd->mdata.push_back(pushd);
	return GetProcAddr(section, main_menu);
}

inline void BuildMenu(BLOCK_INFO blk)
{
	DEBUG << "Building:" << blk.name << " => \"" << blk.father << "\"" << endl;
	MENU *menu = GetProcAddr(blk.father, main_menu);
	if (menu == EMPTY_RET)
	{
		cout << "Try to insert a non exist menu!" << endl;
		cout << blk.father << " is not exist!" << endl;
		exit(-1);
	}
	menu->data.push_back(blk);
}

void BuildMenu(vector<BLOCK_INFO> blk)
{
	DEBUG << "Start to process " << blk.size() << endl;
	for (unsigned long long n = 0; n < blk.size(); n++)
		BuildMenu(blk.at(n));
}