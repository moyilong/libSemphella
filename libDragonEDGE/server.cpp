#include "libDragonEDGE.h"
#include "utils.h"
libDebug srv("EDGEServer");

void service(driver_t &drv)
{
	if (!drv.inited)
		drv.init();
	while (true)
	{
		char buff[IO_LEN];
		long long len=IO_LEN;
		sZero(buff, 0, IO_LEN);
		drv.recv(buff, len);
		IO_BUFF get;
		memcpy(&get, buff, IO_LEN);
		IO_BUFF ret;
		if (!diff_check(get))
		{
			ret.cmdcall = PACKAGE_FAILD;
		}
		else {
			call_modules(ret, get);
		}
	}
}