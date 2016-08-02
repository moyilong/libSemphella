#pragma once

typedef void(*API_CALL)();

class _feature {
public:
	_feature(ASSIGN_TYPE cmd_reg, string reg_name, API_CALL call);
	ASSIGN_TYPE reg;
	string name;
	API_CALL api;
};

extern vector<_feature> cmd_poll;
void exec(string reg_name);
void exec(ASSIGN_TYPE type);

void loadmem(ASSIGN_TYPE ptr, void *rptr);
void setmem(ASSIGN_TYPE ptr, void *rptr);
#define feature static _feature