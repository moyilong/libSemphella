#include "kernel.h"
#include <time.h>
#include "config.h"
bool kernel_inited = false;
bool debug_stat = __DEFAULT_DEBUG_STAT;
kernel KERNEL;
VER kver;
#ifndef __linux__
#include "net.h"
WSAData wsa;
WORD word;
#endif
void kernel::_SetDebugStat(bool stat, string file, int line)
{
#ifndef __ALLOW_DEBUG_STAT_CHANGE
	_message("Debug stat change was been disabled! Track from ", file, line);
#else
	debug_stat = stat;
#endif
}

bool kernel::GetDebugStat()
{
	return debug_stat;
}

kernel::kernel()
{
	srand((unsigned)time(0));
	if (kernel_inited)
		abort();
	start_time = time(0);
	for (int n = 0; n < init_call.size(); n++)
		init_call.at(n)();
	kver.main = 1;
	kver.build = 5;
	kver.version = 0;
	kver.fix = API_VER;
#ifndef __linux__
	word = MAKEWORD(2, 2);
	WSAStartup(word, &wsa);
#endif
}

kernel::~kernel()
{
#ifndef __linux__
	WSACleanup();
#endif
}

void kernel::LogoPrint()
{
	cout << "                 ___         ___                ___  \n               /      /     /   /   /\\    /   /      \n              /----  /     /   /   /  \\  /   /----   \n             /____  /___  /___/   /    \\/   /____    \n";
}

void kernel::Register(REG_TYPE reg, KSAPI api)
{
	switch (reg)
	{
	case INIT:
		init_call.push_back(api);
		break;
	case INEXIT:
		exit_call.push_back(api);
		break;
	}
}

VER kernel::GetVer()
{
	return kver;
}

int kernel::get_api_ver()
{
	return API_VER;
}

time_t kernel::get_start_time()
{
	return time(0) - start_time;
}

void kernel::abort()
{
	this->error("System Faild Was benn Called!");
	exit(-1);
}
void kernel::_message(string info, string file, int line)
{
	if (debug_stat) cout << "[" << file << "][" << line << "]" << info << endl;
}
void kernel::_error(string info, string file, int line)
{
	cout << "[ERROR][" << file << "][" << line << "]" << info << endl;
	exit(-1);
}

int main(int argc, char *argv[])
{
	KERNEL.LogoPrint();
	cout << "libSemphella " << CORE_NAME << " " << kver.to_str() << endl;
	cout << "Generic Defined Buffers Size:" << MAX_BUFF_SIZE << endl;
	string platform = "Windows";
#ifdef __LINUX__
	platform = "Linux";
#ifdef __DRAGONOS
	platform += " with DragonOS";
#endif
#ifdef __ANDROID__
	platofmr += " with Android";
#endif
#endif
#ifdef __ALLOW_LOWMEM_PREFER
	platform += " in low memory devices";
#endif
	cout << "libSemphella -> " << platform << endl;
#ifdef __ALLOW_DYMANIC_DEBUG
	cout << "Dymanic Debug is Enable, Default Debug is ";
#if __DEFAULT_DEBUG_STAT
	cout << "Enable";
#else
	cout << "Disable";
#endif
	cout << endl;
#endif
}