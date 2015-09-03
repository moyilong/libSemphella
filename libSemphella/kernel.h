#pragma once
#include "main.h"
#define KERNEL_VERSION	"Wing-A9"
#define KERNEL_CODEVER	0x995D

class API KERNEL{
public:
	KERNEL();
	string getver();
	bool get_debug_stat();
	bool set_debug_stat(bool stat);
};

extern API KERNEL kernel;