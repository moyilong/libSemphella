#include "stdafx.h"
#include "command.h"

vector<CLI> command_line_poll;

CLI::CLI(string kmain_exec, CLI_KEXEC kfunc)
{
	main_exec = kmain_exec;
	command_line_poll.push_back(*this);
}

CLI_KEXEC CLI::get_func()
{
	return fun;
}

string CLI::get_name()
{
	return main_exec;
}

int get_count()
{
	return command_line_poll.size();
}

CLI get_point(int n)
{
	return command_line_poll.at(n);
}

int __exec(vector<string>dep)
{
	string cli;
	for (int n = 0; n < dep.size(); n++)
	{
		cli.append(dep.at(n));
		cli.append(" ");
	}
	return system(cli.data());
}

CLI _exec("exe", __exec);

int __edit(vector<string>dep)
{
	string exec;
	exec.append("vi ");
	exec.append(dep.at(0));
	return system(exec.data());
}

CLI _edit("edit", __edit);