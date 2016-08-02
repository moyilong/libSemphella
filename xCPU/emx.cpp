#include "stdafx.h"
#include "feature.h"

void __plus()
{
	tmp_code = arg2_buff + arg_buff;
}

void __less()
{
	tmp_code = arg_buff - arg2_buff;
}

void __vertex()
{
	tmp_code = arg_buff*arg2_buff;
}

void __divid()
{
	if (arg_buff == 0)
		err_code = 0x0000E000;
	else
		tmp_code = arg_buff / arg2_buff;
}

feature _plus(0x00000001, "plus", __plus);
feature _less(0x00000002, "less", __less);
feature _vertex(0x00000003, "vertex", __vertex);
feature _divid(0x00000004, "divid", __divid);

void jmp()
{
	cmd_ptr = arg_buff;
	if (arg2_buff != ATYPE_MAX)
		mem_ptr = arg2_buff;
	if (arg_buff != ATYPE_MAX)
		cmd_ptr = arg_buff;
}

feature _jmp(0x00000005, "jmp", jmp);

void save()
{
	setmem(arg_buff, &arg2_buff);
}

void load_arg1()
{
	loadmem(mem_ptr, &arg_buff);
}

void load_arg2()
{
	loadmem(mem_ptr, &arg2_buff);
}

feature _save(0x00000006, "save", save);
feature _load1(0x00000007, "read1", load_arg1);
feature _load2(0x00000008, "read2", load_arg2);

void disp()
{
	cout << (char*)arg_buff;
}

feature _disp(0x00000009, "disp", disp);