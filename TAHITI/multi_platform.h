#include <stdint.h>
#include <inttypes.h>

#ifdef __LINUX__
#define esleep(ms) usleep(ms*1000)
#else
#define esleep(ms) Sleep(ms)
#endif

#ifndef __LINUX__
inline void close(SOCKET conn)
{
	closesocket(conn);
}

#endif