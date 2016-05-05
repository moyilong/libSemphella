#include "configure.h"

#define CFG_LOAD	cout<<"[ConfigLoad]"

bool init_configure(string file, CONFIG &io, bool allow_syntax_error)
{
	if (!io.link.empty() || io.file.is_open())
		return false;
	io.link = file;
	io.file.open(file.data(), ios_base::in|ios_base::out);
	int line = 0;
	while (!io.file.eof())
	{
		line++;
		string buff;
		getline(io.file, buff);
		if (buff.at(0) == '#')
			continue;
		int equal = -1;
		for (unsigned int n = 0; n < buff.size(); n++)
		{
			if (buff.at(n) == '=')
			{
				if (equal == -1)
				{
					equal = n;
				}
				else{
					CFG_LOAD << "File Format Error! @"<<line<<"."<<n << endl;
					CFG_LOAD << "Syntax Error: Double '='"<<endl;
					if (!allow_syntax_error)
						return false;
				}
			}
		}
		BLOCK temp;
		if (equal != -1)
		{
			temp.name = buff.substr(0, equal);
			temp.value = buff.substr(equal + 1, buff.size() - equal - 1);
		}
		else{
			temp.name = buff;
			temp.value = STR_FALSE;
		}
		if (get_value_id(temp.name, io) != -1)
		{
			CFG_LOAD << "Label Name Replace! @" << line << endl;
		}
		else{
			io.poll.push_back(temp);
		}

	}
	return true;
}

int get_value_id(string name, CONFIG &io)
{
	int ret = -1;
#pragma omp parallel for
	for (long n = 0; n < io.poll.size(); n++)
		if (!strcmp(io.poll.at(n).name.data(), name.data()))
			ret = n;
	return ret;
}

string get_value(string name, CONFIG &io)
{
	int ret = get_value_id(name, io);
	if (ret == -1)
		return STR_FALSE;
	else
		return io.poll.at(ret).value;
}

#ifndef __ANDROID__
#include <omp.h>

void for_each(FOR_EACH_API api, CONFIG &io,bool omp)
{
	int mp_thread;
	if (!omp)
	{
		mp_thread = omp_get_thread_num();
		omp_set_num_threads(1);
	}
#pragma omp parallel for
	for (long n = 0; n < io.poll.size(); n++)
		api(io.poll.at(n));
	if (!omp)
		omp_set_num_threads(mp_thread);
}
#else
void for_each(FOR_EACH_API api, CONFIG &io, bool omp)
{
	for (long n = 0; n < io.poll.size(); n++)
		api(io.poll.at(n));
}
#endif