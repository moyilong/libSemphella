#pragma once
#include <libSemphella/main.h>
#include <libSemphella/string.h>
#include <libERT/libERT.h>
#include <libSemphella/sysapi_modcall.h>

#include "APP_HINSTANCE.h"

struct VAPP_STORAGE {
	VAPP *app;
	mod_t link;
};
#define HINSTANCE_RUN(link) if (link != NULL) link

bool load_app(string modname);
bool unload_app(string appname);
uint64_t GetInsertModules();
VAPP* GetApplication(uint64_t id);
VAPP* GetApplication(string name);
int RunApplication(string name);