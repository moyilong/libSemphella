#pragma once

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
#pragma pack(push,1)
typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
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
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)
class CBmpReader {
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

private:
	PBITMAPFILEHEADER m_pFileHeader;
	PBITMAPINFOHEADER m_pInfoHeader;
	char * m_buffer;
	char * m_pRawData;
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	unsigned int m_nLineBytes;
};

