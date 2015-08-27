// time_lize.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libSemphella/apd.h>
#include <libSemphella/string.h>
using namespace APD_UTILS;
string script_dir;
string config_dir;

libDebug time_lize("TimeLize");
APD config;

/*CONFIG*/
/*
[index:<name>]
hors=运行小时
second=秒偏差
count=技术(-1为不限制)
*/

struct INDEX{
	int hors=-1;
	int second=-1;
	int count = -1;
};

vector<INDEX> poll;

bool checker(string data)
{
	return streval("index:", data.substr(0, strlen("index:")).data());
}

void load(label lab, INDEX &temp)
{
	if (streval(lab.name.data(), "hors"))
	{
		temp.hors = atoi(lab.data.data());
		return;
	}
	if (streval(lab.name.data(), "second"))
	{
		temp.second = atoi(lab.data.data());
		return;
	}
}

void for_each(NODE link, COUNT_TYPE nid)
{
	INDEX temp;
	for (int n = 0; n < link.label.size(); n++)
		load(link.label.at(n), temp);
	if (temp.hors == -1 && temp.second == -1)
	{
		time_lize << "Load Node Faild!!" << endl << "Link:" << link.n_name << endl << "Time Set Error!" << endl;
	}
	else{
		poll.push_back(temp);
	}
}

#include <time.h>

int main(int argc, char* argv[])
{
	script_dir = SCRIPT_DIR;
	if (script_dir.empty())
	{
		script_dir = _DEF_SCRIPT_DIR;
	}
	config_dir = CONFIG_DIR;
	if (config_dir.empty())
	{
		config_dir = _DEF_CONFIG_DIR;
	}
	time_lize << "Inited Enviroment" << endl << "Script Dir:" << script_dir << endl << "Config Dir:" << config_dir << endl;
	config.load(config_dir + "//time_lize.cfg");
	config.node_for_each(for_each, checker, false);
	while (true)
	{
		time_t t = time(0);
		tm *ti = localtime(&t);
		char buff[MAX_BUFF_SIZE] = { 0 };
		strftime(buff, MAX_BUFF_SIZE, "%Y", ti);
		int year = atoi(buff);
	}
}