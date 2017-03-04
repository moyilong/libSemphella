#pragma once
#include "main.h"
#include "string.h"
#include "debug.h"
#include "kernel.h"

#ifdef _WINDOWS
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#elif defined(__linux__)

#endif

enum Party {
	EVEN,	//żУ��
	NONE,	//��У��
	MARK,	//���У��
	ODD,	//��У��
};

struct SerialConfig {
	int speed;
	int data_bit = 8;
	int stop_bit = 0;
	Party parity = NONE;
};

class API Serial {
public:
	void SetPath(const string xpath);

	void close();
	void open();

	void open(const string xpath);
	void open(const SerialConfig xcfg);
	void open(const string xpath, const SerialConfig xcfg);
	void SetConfig(const SerialConfig xcfg);
	SerialConfig GetConfig();
	bool is_opened();

	void read(char *buff, int &len);
	void write(const char *buff, const int len);
private:
	SerialConfig cfg;
	string path;
#ifdef _WINDOWS
	HANDLE 
#elif defined(__linux__)
	int
#endif
	handle;
	bool status = false;
};