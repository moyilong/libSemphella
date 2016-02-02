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
					CreateMenu(readd.section, readd.display, readd.father);
				else
					bpoll.push_back(readd);
				readd.empty();
				readd.name=getLine.substr(1, readd.name.size() - 2);
			}
			else
				first = !first;
		}
		else {
			string name;
			string value;
			int eq = strfind(getLine.data(), '=', true);
			name = getLine.substr(0, eq);
			value = getLine.substr(eq + 1);
			if (streval(name.data(), "uci_info"))
				readd.uci_info = value;
			else if (streval(name.data(), "type"))
				readd.type = value;
			else if (streval(name.data(), "depends"))
				readd.depends = value;
			else if (streval(name.data(), "prompt"))
				readd.type = getLine.substr(eq + 1);
			else if (streval(name.data(), "section"))
				readd.section = value;
			else if (streval(name.data(), "display"))
				readd.display = value;
			else if (streval(name.data(), "father"))
				readd.father = value;
			else if (streval(name.data(), "exec"))
				readd.systemd = value;
			else {
				cout << "Error: Unknow Label Append!" << endl;
				cout << filename << "@" << count << endl;
			}
		}
	}
	if (streval(readd.type.data(), "menu"))
		CreateMenu(readd.section, readd.display, readd.father);
	else
		bpoll.push_back(readd);
	for (long long n = 0; n < bpoll.size(); n++)
		bpoll.at(n).default_val = get_config(bpoll.at(n).uci_info);
	BuildMenu(bpoll);
}