
struct KERNEL{
	bool loop_continue = true;
	string work_dir = "";
	string pwd = "//DragonOS/";
	KERNEL();
};

KERNEL &kernel();


typedef int(*CLI_KEXEC)(vector<string>arg);

class CLI{
public:
	inline CLI(){};
	CLI(string kmain_exec, CLI_KEXEC kfunc);	
	CLI_KEXEC get_func();
	string get_name();
private:
	string main_exec;
	CLI_KEXEC fun;

};

int get_count();
CLI get_point(int n);
string cli_head();