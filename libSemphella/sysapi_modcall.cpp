#include "sysapi_modcall.h"

API  void CloseLibrary(mod_t mod)
{
#ifdef __LINUX__
	dlclose(mod);
#else
	FreeLibrary(mod);
#endif
}

API void * GetLibraryAddress(mod_t mod, string name)
{
	debug << "Load proc from:" << name << endl;
#ifdef __LINUX__
	return dlsym(mod, name.data());
#else
	return GetProcAddress(mod, name.data());
#endif
}

API mod_t OpenLibrary(string modname)
{
	debug << "Opening Library:" << modname << endl;
#ifdef __LINUX__
	return dlopen(modname.data(), RTLD_NOW);
#else
	return LoadLibraryA(modname.data());
#endif
}
