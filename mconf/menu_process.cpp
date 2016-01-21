#include "stdafx.h"
#include "loaddef.h"



MENU GetProcAddr(string section,MENU &search_from)
{

	if (streval(section.data(), search_from.section.data()))
		return search_from;
	MENU ret= EMPTY_RET;
#pragma omp parallel for
	for (long long n = 0; n < search_from.mdata.size(); n++)
	{
		MENU temp = GetProcAddr(section, search_from);
		if (temp != EMPTY_RET)
			ret = temp;
	}
	return ret;
}

MENU CreateMenu(string section, string display,string father)
{
	MENU getd = GetProcAddr(section, main_menu);
	if (getd != EMPTY_RET)
		return getd;
	getd = GetProcAddr(father, main_menu);
	if (getd == EMPTY_RET)
	{
		DEBUG << "Faild Create Menu form : " << father << endl;
		exit(-1);
	}
	MENU pushd;
	pushd.section = section;
	pushd.display_name = display;
	getd.mdata.push_back(pushd);
	return GetProcAddr(section, main_menu);
}

inline void BuildMenu(BLOCK_INFO blk)
{
	MENU getd = CreateMenu(blk.section, blk.display, blk.father);
	getd.data.push_back(blk);

}

void BuildMenu(vector<BLOCK_INFO> blk)
{
	for (unsigned long long n = 0; n < blk.size(); n++)
		BuildMenu(blk.at(n));
}