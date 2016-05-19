#include "libVAPP.h"
vector<VAPP_STORAGE> vapp;

bool load_app(string modname)
{
	VAPP_STORAGE stor;
	stor.link = OpenLibrary(modname);
	if (stor.link == NULL)
		return false;
	stor.app = (VAPP*)GetLibraryAddress(stor.link, "__NAMED_API_LINK");
	if (stor.app == NULL)
	{
		CloseLibrary(stor.link);
		return false;
	}
	if (stor.app->api_ver != VAPP_API_VER)
	{
		CloseLibrary(stor.link);
		return false;
	}
	HINSTANCE_RUN(stor.app->init)();
	vapp.push_back(stor);
	return true;
}

bool unload_app(string appname)
{
	for (int n=0;n<vapp.size();n++)
		if (streval(vapp.at(n).app->appname.data(), appname.data()))
		{
			HINSTANCE_RUN(vapp.at(n).app->exit)();
			CloseLibrary(vapp.at(n).link);
			vapp.erase(vapp.begin() + n);
			return true;
		}
	return false;
}

uint64_t GetInsertModules()
{
	return vapp.size();
}

VAPP* GetApplication(string name)
{
	for (int n = 0; n < vapp.size(); n++)
		if (streval(vapp.at(n).app->appname.data(), name.data()))
			return vapp.at(n).app;
	return NULL;
}

inline int run_app(VAPP *app,int argc,char **argv)
{
	if (app->entry == NULL)
		return false;
	try {
		return app->entry(argc, argv);
	}
	catch (exception e)
	{
		return -1;
	}
	catch (...)
	{

	}
}

int RunApplication(string name,int argc,char *argv[])
{
	return run_app(GetApplication(name),argc,argv);
}

VAPP* GetApplication(uint64_t id)
{
	for (int n = 0; n < vapp.size(); n++)
		if (vapp.at(n).app->uuid == id)
			return vapp.at(n).app;
	return NULL;
}