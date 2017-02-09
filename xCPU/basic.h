#pragma once
#undef error
void init();
void error();
void poweroff();
void reset();
void loop();

extern ASSIGN_TYPE cmd_ptr;
extern ASSIGN_TYPE mem_ptr;
extern ASSIGN_TYPE tmp_code;
extern ASSIGN_TYPE err_rst;
extern ASSIGN_TYPE mem_buff;
extern ASSIGN_TYPE cmd_buff;
extern ASSIGN_TYPE arg_buff;
extern ASSIGN_TYPE arg2_buff;
extern ASSIGN_TYPE err_code;
extern ASSIGN_TYPE auto_rst;
extern ASSIGN_TYPE rst;
extern ASSIGN_TYPE mem_rst;
extern ASSIGN_TYPE cloop;
extern ASSIGN_TYPE bool_buff;
extern bool bool_poll[BOOL_VAL_AREA];
struct CMD {
	ASSIGN_TYPE type;
	ASSIGN_TYPE arg;
	ASSIGN_TYPE arg2;
	ASSIGN_TYPE bptr;
};

extern vector<CMD>program;

inline bool bool_get()
{
	if (bool_buff == 0)
		return true;
	if (bool_buff == BOOL_VAL_AREA - 1)
		return false;
	return bool_poll[bool_buff];
}