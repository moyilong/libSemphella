#pragma once
#include "main.h"

#ifdef __LINUX__
typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
#endif
#pragma pack(push,1)

struct __tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
};

struct __tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
};

#define BITMAPFILEHEADER __tagBITMAPFILEHEADER
#define  BITMAPINFOHEADER __tagBITMAPINFOHEADER
#define PBITMAPFILEHEADER __tagBITMAPFILEHEADER*
#define PBITMAPINFOHEADER __tagBITMAPINFOHEADER*

#pragma pack(pop)
struct RGB_COLOR {
	unsigned int R;
	unsigned int G;
	unsigned int B;
};

class CAPI CBmpReader {
public:
	CBmpReader();
	~CBmpReader();

public:
	int Load(const char * bmp_file);
	void Unload();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	PBITMAPFILEHEADER GetFileHeader() const;
	PBITMAPINFOHEADER GetInfoHeader() const;

	char * GetRawData() const;

	unsigned int GetRedAt(int row, int col) const;
	unsigned int GetGreenAt(int row, int col) const;
	unsigned int GetBlueAt(int row, int col) const;
	RGB_COLOR GetColorAt(int row, int col) const;
	

private:
	PBITMAPFILEHEADER m_pFileHeader;
	PBITMAPINFOHEADER m_pInfoHeader;
	char * m_buffer;
	char * m_pRawData;
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	unsigned int m_nLineBytes;
};

API void  OutputBitMapInfoHeader(PBITMAPINFOHEADER pInfoHeader);
API void OutputBitMapFileHeader(PBITMAPFILEHEADER pFileHeader);