#include "libERT.h"
#include "algorthim.h"
#include "fhandle.h"
#include "external_library.h"
#include "ext_io.h"

struct  storaged_modules_info {
	modules_info *info;
	~storaged_modules_info();
	mod_t hmod;
};
  vector<storaged_modules_info> modpoll;

LIBERT_API int  LoadExternalLib(string filename)
{
	cp2 << "Open Library File:" << filename << endl;
	storaged_modules_info info;
	cp2 << "Running Open Funcation..." << endl;
	info.hmod = OpenLibrary(filename.data());
	if (info.hmod == NULL)
	{
		cp2 << "Warring of Load Library File!" << endl;
		return -1;
	}
	info.info = (modules_info*)GetLibraryAddress(info.hmod, "__mod_info_load");
	if (info.info == NULL)
	{
		cp2 << "Warring of Load Procd Function!" << endl;
		CloseLibrary(info.hmod);
		return -1;
	}
	cp2 << "Tips: Loadlibrary:" << info.info->name << endl;
	if (info.info->algr_size == 0 && info.info->fhand_size == 0)
	{
		CloseLibrary(info.hmod);
		cp2 << "No Feature Report! Drop it!" << endl;
		return 0;
	}
	for (int n = 0; n < info.info->fhand_size; n++)
		FHANDLE(info.info->handle_type[n]);
	for (int n = 0; n < info.info->algr_size; n++)
		ALGHRTHIM(info.info->algr_type[n]);
	modpoll.push_back(info);
	return 0;
}



storaged_modules_info::~storaged_modules_info()
{
	CloseLibrary(hmod);
}
