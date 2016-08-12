#pragma once
#include "main.h"
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef signed long		LONG;

#define BI_RGB			0L

#pragma pack(push, 2)

typedef API struct
{
	WORD    bfType=0x4d42;
	DWORD   bfSize;
	WORD    bfReserved1=0;
	WORD    bfReserved2=0;
	DWORD   bfOffBits;
} BMP_FILE_HEAD;

typedef API struct
{
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BMP_INFO_HEAD;

#pragma pack(pop)

