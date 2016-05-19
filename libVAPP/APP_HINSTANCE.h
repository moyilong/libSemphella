#pragma once
#define VAPP_API_VER	0x50

typedef void(*vapp_proc_fun)(void);
typedef int(*vapp_proc_main)(int argc, char **);


struct VAPP {
	char api_ver = VAPP_API_VER;
	string appname;
	string appdescript;
	vapp_proc_fun init;
	vapp_proc_fun exit;
	vapp_proc_main entry;
	uint64_t uuid;
};

#ifndef __LINUX__
#define EXPORTD_LINK	__declspec(dllexport)
#else
#define EXPORTD_LINK
#endif

#define LINKAPI 	extern EXPORTD_LINK VAPP __NAMED_API_LINK