#pragma once
#include "stdafx.h"

#define MAPI_LEVEL	0xFF5B

struct BLOCK_INFO {
	string uci_info;		//UCI指向
	string prompt;			//显示名称
	string type;			//类型
	string depends;			//需求
	string name;			//类名
	string section;			//菜单唯一标识符
	string display;			//显示菜单名称
	string father;			//母菜单标识符
	string default_val;		//默认值
	string systemd;			//执行命令
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
		default_val = "";
		systemd="";
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

MENU *CreateMenu(string section, string display, string father);
MENU *GetProcAddr(string section, MENU &search_from);
void BuildMenu(vector<BLOCK_INFO> blk);
//#define EMPTY_RET NULL_MENU
#define EMPTY_RET NULL
void BuildMenu(vector<BLOCK_INFO> blk);
void Prompt(string filename);
void compile(string filename);
//Cache Option
void Write(string filename);
void Read(string filename);
void Prompt_DotConfig(string filename);
inline bool operator ==(MENU a, MENU b)
{
	if (!streval(a.section.data(),b.section.data()))
		return false;

	if (!streval(a.display_name.data(), b.display_name.data()))
		return false;
	return true;
}

inline bool operator !=(MENU a, MENU b) { return !(a == b); }

struct CONFIG {
	string name;
	string data;
};

extern vector<CONFIG> confpoll;
string get_config(string uci);
string set_config(string uci, string value);
void Apply(string input);