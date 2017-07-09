#pragma once
#include "main.h"
#include "debug.h"
#ifndef __LINUX__
#include <Windows.h>
#define mod_t HINSTANCE
#else
#include <dlfcn.h>
#define mod_t void*
#endif

API void CloseLibrary(mod_t mod);
API void * GetLibraryAddress(mod_t mod, string name);
API mod_t OpenLibrary(string modname);