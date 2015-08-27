#include "../libDragonEDGE.h"


void __power_oper(IO_BUFF &io, const IO_BUFF get)
{
	switch (get.swap[0])
	{
	case 'd':
#ifdef __LINUX__
		system("poweroff");
#else
		system("shutdown -s -t 0");
#endif
		break;
	case 'r':
#ifdef __LINUX__
		system("reboot");
#else
		system("shutdown -r -t 0");
#endif
		break;
	default:
		break;
	}
}

modules power_oper(0xFFFF1000, __power_oper);
