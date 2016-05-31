#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <libSemphella/argment.h>
using namespace std;
#include <libSemphella/VER.h>
#define APP_VER VER

typedef int(*MAIN_ENTRY)(argment args);
typedef void(*PROC_TYPE)();
struct APP_ENTRY {
	string appname;
	APP_VER ver;
	APP_VER pg_ver = APP_VER(0, 6, 1, 1);
	MAIN_ENTRY main_ent;
	PROC_TYPE init_call;
	PROC_TYPE end_call;
	inline APP_ENTRY(string name, APP_VER v, MAIN_ENTRY main, PROC_TYPE in, PROC_TYPE ex)
	{
		appname = name;
		ver = v;
		main_ent = main;
		init_call = in;
		end_call = ex;
	}
};
