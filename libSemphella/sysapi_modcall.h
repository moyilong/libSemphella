#pragma once
#include "main.h"
#ifndef __LINUX__
#include <Windows.h>
#define mod_t HINSTANCE
#else
#include <dlfcn.h>
#define mod_t void*
#endif
inline  void CloseLibrary(mod_t mod)
{
#ifdef __LINUX__
	dlclose(mod);
#else
	FreeLibrary(mod);
#endif
}

inline void * GetLibraryAddress(mod_t mod, string name)
{
	debug << "Load proc from:" << name << endl;
#ifdef __LINUX__
	return dlsym(mod, name.data());
#else
	return GetProcAddress(mod, name.data());
#endif
}

inline mod_t OpenLibrary(string modname)
{
	debug << "Opening Library:" << modname << endl;
#ifdef __LINUX__
	return dlopen(modname.data(), RTLD_NOW);
#else
	return LoadLibraryA(modname.data());
#endif
}