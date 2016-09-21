#pragma once
#include <libSemphella/string.h>
#include <libSemphella/debug.h>
#include <libSemphella/files.h>



#ifndef __LINUX__

#ifndef QR_ALL
#define QRAPI _declspec(dllexport)
#else
#define QRAPI _declspec(dllimport)
#endif

#else

#define QRAPI

#endif

#include <libSemphella/BMP.h>
#define BI_RGB 0L
QRAPI void CreateBMPBUff(string data, char *ptr, uint64_t &len, uint64_t prescale);
QRAPI void CreateDataBuff(const char *data, uint64_t dlen, char *ptr, uint64_t &len, uint64_t prescale);
