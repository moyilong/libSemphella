#pragma once
#ifndef __LINUX__
#define IO_TYPE	HANDLE
#else
#endif

struct DEVICE_STAT{
	bool parity = false;
	int speed = 115200;
	int byte_size = 8;
	int stop_bit = 0;
};

IO_TYPE init(string port, DEVICE_STAT stat);
void close(IO_TYPE port);
int read(IO_TYPE io, char *buff, size_t read_len);
int write(IO_TYPE io,const char *buff, size_t send_len);