#pragma once
#include "main.h"

#define ARG_NOT_EXIST	"__not_exist___"

struct CONFIG_BLOCK {
	string name;
	string value;
};

typedef void(*for_each_fun)(string value, string data);

class CAPI argment
{
public:
	argment();
	~argment();
	string get_config(string name);
	void load(int argc, char *argv[]);
	int size();
	CONFIG_BLOCK at(int lid);
	bool define(string name);
	int get_id(string name);
	void for_each(for_each_fun fun);
	void add(string name, string val);
private:
	vector<CONFIG_BLOCK>block;
};
