#pragma once
#ifndef __LINUX
#define IO_TYPE	HANDLE
#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#define IO_TYPE int
#define INVALID_HANDLE_VALUE	-1
#endif

struct DEVICE_STAT{
	bool parity = false;
	int speed = 115200;
	int byte_size = 8;
	int stop_bit = 1;
};

IO_TYPE pinit(string port, DEVICE_STAT stat);
void pclose(IO_TYPE port);
int pread(IO_TYPE io, char *buff, size_t read_len);
int pwrite(IO_TYPE io,const char *buff, size_t send_len);
