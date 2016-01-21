#pragma once
#include "stdafx.h"


struct BLOCK_INFO {
	string uci_info;		//UCIָ��
	string prompt;			//��ʾ����
	string type;			//����
	string depends;			//����
	string name;			//����
	string section;			//�˵�Ψһ��ʶ��
	string display;			//��ʾ�˵�����
	string father;			//ĸ�˵���ʶ��
	inline BLOCK_INFO()
	{
		uci_info = "";
		prompt = "";
		type = "config";
		depends = "";
		name = "";
		section = "";
		display = "";
		father = "main";
	}
	inline void empty()
	{
		BLOCK_INFO();
	}
};

struct MENU {
	string section;
	string display_name;
	vector<BLOCK_INFO> data;
	vector<MENU> mdata;
};

extern MENU NULL_MENU;

extern MENU main_menu;

MENU CreateMenu(string section, string display, string father);
MENU GetProcAddr(string section, MENU &search_from);
void BuildMenu(vector<BLOCK_INFO> blk);
#define EMPTY_RET NULL_MENU
void BuildMenu(vector<BLOCK_INFO> blk);
void Prompt(string filename);
void compile(string filename);

inline bool operator ==(MENU a, MENU b)
{
	if (!streval(a.section.data(),b.section.data()))
		return false;

	if (!streval(a.display_name.data(), b.display_name.data()))
		return false;
	return true;
}

inline bool operator !=(MENU a, MENU b) { return !(a == b); }