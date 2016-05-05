#pragma once
#include "../stdafx.h"

#define STR_TRUE	"y"
#define STR_FALSE	"n"

#ifndef MAX_EVENT_SIZE
#define MAX_EVENT_SIZE	4*MAX_BUFF_SIZE
#endif

struct BLOCK{
	string name;
	string value;
};

typedef void(*FOR_EACH_API)(const BLOCK info);

struct CONFIG{
	vector<BLOCK> poll;
	string link;
	fstream file;
	inline ~CONFIG()
	{
		if (file.is_open())
			file.close();
		if (!poll.empty())
			poll.clear();
	}
};

bool init_configure(string file,CONFIG &io,bool allow_syntax_error=true);
string get_value(string name,CONFIG &io);
int get_value_id(string name, CONFIG &io);
void for_each(FOR_EACH_API api, CONFIG &io,bool omp=true);