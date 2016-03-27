#include "stdafx.h"
#include "feature.h"
ASSIGN_TYPE cmd_ptr=0;
ASSIGN_TYPE mem_ptr = 0;
ASSIGN_TYPE tmp_code = 0;
ASSIGN_TYPE err_rst = 0;
ASSIGN_TYPE mem_buff=0;
ASSIGN_TYPE cmd_buff = 0;
ASSIGN_TYPE arg_buff = 0;
ASSIGN_TYPE arg2_buff = 0;
ASSIGN_TYPE err_code = 0;
ASSIGN_TYPE rst = -1;
ASSIGN_TYPE auto_rst = 0;
ASSIGN_TYPE mem_rst = 0;
ASSIGN_TYPE cloop = 0;
vector<CMD> program;
char memory[ALLOW_SIZE];

void info()
{
#define disp_st(ptr,str) cout<<"Storage State:"<<str<<" == "<<ull2s(ptr)<<endl;
	disp_st(cmd_ptr, "CMD_PTR");
	disp_st(mem_ptr, "MEM_PTR");
	disp_st(cmd_buff, "CMD_BUFF");
	disp_st(mem_buff, "MEM_BUFF");
	disp_st(arg_buff, "ARG1");
	disp_st(arg2_buff, "ARG2");
	disp_st(tmp_code, "TempCode");
	disp_st(err_code, "ErrorCode");
	disp_st(rst, "Resetstat");
	disp_st(auto_rst, "AutoReset");
	disp_st(mem_rst, "Resetting Memory");
	disp_st(err_rst, "Error Reset Count");
	cout << "MemSize:" << human_read(ALLOW_SIZE, human_read_storage_str,1024,10)<<endl;

}


void poweroff()
{
	cout << "System Halt!" << endl;
	exit(0);
}

void init()
{
	if (rst != 0)
		reset();
	info();
}

void error()
{
	cout << "System Hardware Error Call!" << endl;
	rst = -2;
	if (err_rst > 100)
		poweroff();
	if (auto_rst)
	{
		err_rst++;
		cout << "System will be reboot!" << endl;
		esleep(500);
		init();
		loop();
	}
}

void reset()
{
	rst = 1;
	cout << "System Resetting..." << endl;
	cmd_ptr = 0;
	cmd_buff = 0;
	mem_ptr = 0;
	tmp_code = 0;
	mem_buff = 0;
	arg_buff = 0;
	arg2_buff = 0;
	cloop = 0;
	cout << "Resetting Secure Zone.." << endl;
	for (int n = 0; n < SECURE_ZONE_SIZE; n++)
		memory[n+MEM_SIZE] = 0;
	cout << "Parallel Resetting Memory!" << endl;
	if (mem_rst == 0)
	{
#pragma omp parallel for
		for (SIGNED_TYPE n = 0; n < MEM_SIZE; n++)
		{
			ASSIGN_TYPE ptr = n;
			memory[ptr] = 0;
			if (n % 1024 == 0)
				cout << "Restting Memory:" << ull2s(n) << "\r";
		}
		cout << endl;
		mem_rst = 1;
	}
	cout << "Reset Over!" << endl;
	auto_rst = 1;
	rst = 0;
}

void loop()
{
	cout << "Inter Loop Mode!" << endl;
#define ERR_CHECK if (err_code != 0) break;
	if (program.size() == 0)
	{
		err_code = 0xC0000001;
	}
	while (err_code == 0)
	{
		if (cmd_ptr > program.size())
			err_code = 0xB0000000;
		if (mem_ptr > MEM_SIZE)
			arg_buff = program.at(cmd_ptr).arg;
		arg2_buff = program.at(cmd_ptr).arg2;
		cmd_buff = program.at(cmd_ptr).type;
		loadmem(mem_ptr, &mem_buff);
		ERR_CHECK;
		exec(cmd_buff);
		cmd_ptr++;
		mem_ptr++;
	}
	cout << "Exit from Loop!" << endl;
	cout << "Last Return Error Code:" << ull2s(err_code) << endl;
	cout << "Program:" << ull2s(cmd_ptr) << endl;
	error();
}

void loadmem(ASSIGN_TYPE ptr, void *rptr)
{
	if (ptr + ARCH_LENGTH > MEM_SIZE)
	{
		err_code = 0xCCCC0000;
		return;
	}
	if (ptr < 0)
	{
		err_code = 0xCCCC0001;
		return;
	}
	memcpy(rptr, memory+ptr, ARCH_LENGTH);
}

void setmem(ASSIGN_TYPE ptr, void *rptr)
{
	if (ptr + ARCH_LENGTH > MEM_SIZE)
	{
		err_code = 0xCCCC0000;
		return;
	}
	if (ptr < 0)
	{
		err_code = 0xCCCC0001;
		return;
	}
	memcpy(memory + ptr, rptr, ARCH_LENGTH);
}