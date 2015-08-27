#include "libDragonEDGE.h"
#include "utils.h"

void client(driver_t &drv)
{
	if (!drv.inited)
		drv.init();
	while (true)
	{
		long long len=IO_LEN;
		IO_BUFF last_get;
		IO_BUFF last_send;
		for (int n = 0; n < getpoll().size(); n++)
		{
			IO_BUFF buff;
			getpoll().at(n).ent(buff, last_get);
			if (!buff.cmdcall != 0)
			{
				last_send = buff;
				write_check(buff);
				long long len = IO_LEN;
				drv.send((char*)&buff,len);
				IO_BUFF getd;
				do {
					char dbuff[IO_LEN];
					drv.recv(dbuff, len);
					memcpy(&getd, dbuff, IO_LEN);
				} while (!diff_check(getd));
				if (getd.cmdcall == PACKAGE_FAILD)
				{
					do {
						char dbuff[IO_LEN];
						drv.recv(dbuff, len);
						memcpy(&getd, dbuff, IO_LEN);
					} while (!diff_check(getd));
				}
				last_get = getd;
				
			}
		}


	}
}