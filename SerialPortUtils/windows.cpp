#include "stdafx.h"
#include "api.h"

#ifndef __LINUX__

IO_TYPE pinit(string port, DEVICE_STAT stat)
{
	cout << "WorkPort:" << port << endl;
	IO_TYPE ret;
	ret = CreateFile(port.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (ret == INVALID_HANDLE_VALUE)
		return NULL;
	DCB com;
	if (!GetCommState(ret, &com))
	{
		cout << "Read Device Stat Faild!" << endl;
		return NULL;
	}
	com.BaudRate = stat.speed;
	com.fBinary = true;
	com.fParity = stat.parity;
	com.fOutxCtsFlow = false;
	com.fOutxDsrFlow = false;
	com.fDtrControl = RTS_CONTROL_DISABLE;
	com.fDsrSensitivity = false;
	com.fTXContinueOnXoff = true;
	com.fOutX = false;
	com.fInX = false;
	com.fErrorChar = false;
	com.fRtsControl = RTS_CONTROL_DISABLE;
	com.ByteSize = stat.byte_size;
	com.StopBits = stat.stop_bit;
	if (!SetCommState(ret, &com))
		return NULL;

	return ret;
}

void pclose(IO_TYPE port)
{
	CloseHandle(port);
}

int pread(IO_TYPE io, char *buff, size_t read_len)
{
	DWORD ret = 0;
	ReadFile(io, buff, read_len, &ret, NULL);
	return ret;
}

int pwrite(IO_TYPE io, const char *buff, size_t send_len)
{
	cout << "Write data to Port:" << buff << endl;
	cout << "Send Size:" << send_len << endl;
	DWORD send = 0;
	WriteFile(io, buff, send_len, &send, NULL);
	cout << "Real Send Len:" << send << endl;
	Sleep(500);
	return send;
}

#endif