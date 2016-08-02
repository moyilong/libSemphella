#include "stdafx.h"
#include "loaddef.h"

void compile(string filename)
{
	cout << "CC\t" << filename << endl;
	vector<BLOCK_INFO>bpoll;
	ifstream input;
	input.open(filename.data());
	if (!input.is_open())
	{
		DEBUG << "Open File:" << filename << "Faild!" << endl;
		exit(0);
	}
	uint64_t count = 0;
	BLOCK_INFO readd;
	bool first = true;
	while (!input.eof())
	{
		count++;
		string getLine;
		getline(input, getLine);
		if (getLine.empty())
			continue;
		if (getLine.at(0) == '#')
			continue;
		if (getLine.at(0) == '!')
		{
			compile(getLine.substr(1));
			continue;
		}
		getLine = strrm(getLine.data(), " \n\r\t\"");
		//getLine = space_fix(getLine);
		bool empty_check = true;
		for (int n = 0; n < getLine.size(); n++)
			if (getLine.at(n) != ' ')
				empty_check = false;
		if (empty_check || getLine.empty())
			continue;
		if (getLine.at(0) == '[' && getLine.at(getLine.size() - 1) == ']')
		{
			if (!first)
			{
				if (streval(readd.type.data(), "menu"))
				{
					DEBUG << "Redisted menu:" << readd.section << " " << readd.display << " " << readd.father << endl;
					CreateMenu(readd.section, readd.display, readd.father);
				}
				else
				{
					static BLOCK_INFO info;
					info = readd;
					DEBUG << info.name << "@" << info.father << " Was been reg!" << endl;
					bpoll.push_back(info);
				}
				readd.empty();
			}
			else
			{
				first = !first;
			}
			readd.name = getLine.substr(1, getLine.size() - 2);
			DEBUG << "Start to process:" << readd.name << endl;
		}
		else {
			string name;
			string value;
			int eq = strfind(getLine.data(), '=', true);
			name = getLine.substr(0, eq);
			value = getLine.substr(eq + 1);
			DEBUG << "get value:" << name << " = " << value << endl;
			if (first)
			{
				if (streval(name.data(), "main_menu_name"))
					main_menu.display_name = value;
				continue;
			}
			if (streval(name.data(), "uci_info"))
				readd.uci_info = value;
			else if (streval(name.data(), "type"))
				readd.type = value;
			else if (streval(name.data(), "depends"))
				readd.depends = value;
			else if (streval(name.data(), "prompt"))
				readd.prompt = getLine.substr(eq + 1);
			else if (streval(name.data(), "section"))
				readd.section = value;
			else if (streval(name.data(), "display"))
				readd.display = value;
			else if (streval(name.data(), "father"))
				readd.father = value;
			else if (streval(name.data(), "exec"))
				readd.systemd = value;
			else if (streval(name.data(), "default_val"))
				readd.default_val = value;
			else {
				cout << "Error: Unknow Label Append!" << endl;
				cout << filename << "@" << count << endl;
				cout << getLine << endl << endl;
			}
		}
	}
	if (streval(readd.type.data(), "menu"))
		CreateMenu(readd.section, readd.display, readd.father);
	else
		bpoll.push_back(readd);
	DEBUG << "Splite Complete! Start Memory  Process..." << endl;
	DEBUG << "Count of block:" << bpoll.size() << endl;
	BuildMenu(bpoll);
}