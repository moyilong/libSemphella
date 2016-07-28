#pragma once
#include "CALL_TYPE.h"

#ifndef __LINUX__
#define DLL_EXPORT_TYPE	__declspec(dllexport)
#else
#define DLL_EXPORT_TYPE
#endif

#define MODULES DLL_EXPORT_TYPE APP_ENTRY __main_entry_info
