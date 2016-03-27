#pragma once
void init();
void error();
void poweroff();
void reset();
void loop();


extern ASSIGN_TYPE cmd_ptr ;
extern ASSIGN_TYPE mem_ptr ;
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
extern ASSIGN_TYPE loop;

struct CMD {
	ASSIGN_TYPE type;
	ASSIGN_TYPE arg;
	ASSIGN_TYPE arg2;
};

extern vector<CMD>program;