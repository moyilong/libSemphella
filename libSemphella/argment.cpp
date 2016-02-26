#include "argment.h"
#include "debug.h"


argment::argment()
{
}


argment::~argment()
{
	block.clear();
}

string argment::get_config(string name)
{
	if (!define(name))
		return ARG_NOT_EXIST;
	return block.at(get_id(name)).value;
}

void argment::load(int argc, char * argv[])
{
	debug << "Import Argment:" << argc << endl;
	for (int n = 0; n < argc; n++)
		if (argv[n][0] == '/' || argv[n][0] == '-')
		{
			CONFIG_BLOCK blk;
			blk.name = argv[n]+1;
			if (n == argc - 1 || argv[n + 1][0] == '/' || argv[n + 1][0] == '-')
				blk.value = "def";
			else
			{
				n++;
				blk.value = argv[n];
			}
			debug << blk.name << " == " << blk.value <<" was been import "<< endl;
			block.push_back(blk);
		}
}

int argment::size()
{
	return block.size();
}

CONFIG_BLOCK argment::at(int lid)
{
	return block.at(lid);
}

bool argment::define(string name)
{
	return !(get_id(name) == -1);
}
#include <string>
#include "string.h"
int argment::get_id(string name)
{
	for (int n = 0; n < block.size(); n++)
		if (streval(name.data(), block.at(n).name.data()))
			return n;
	return -1;
}

void argment::for_each(for_each_fun fun)
{
	for (int n = 0; n < block.size(); n++)
		fun(block.at(n).name, block.at(n).value);
}
