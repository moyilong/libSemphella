#include "BMP.h"
#include <stdio.h>

CBmpReader::CBmpReader() {
	m_pFileHeader = NULL;
	m_pInfoHeader = NULL;
	m_buffer = NULL;
	m_pRawData = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nLineBytes = 0;
}

CBmpReader::~CBmpReader() {
	Unload();
}

#define FOUR_BYTES_ALIGN(width_in_bit)  ((((width_in_bit)+31)>>5)<<2)

int CBmpReader::Load(const char * bmp_file) {
	FILE * pf = fopen(bmp_file, "rb");
	if (NULL == pf) {
		return 0;
	}

	fseek(pf, 0, SEEK_END);
	long length = ftell(pf);
	rewind(pf);
	m_buffer = new char[length];
	fread(m_buffer, length, 1, pf);

	m_pFileHeader = (PBITMAPFILEHEADER)(m_buffer);
	m_pInfoHeader = (PBITMAPINFOHEADER)(m_buffer + sizeof(BITMAPFILEHEADER));
	m_pRawData = m_buffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	m_nWidth = m_pInfoHeader->biWidth;
	m_nHeight = m_pInfoHeader->biHeight;
	m_nLineBytes = FOUR_BYTES_ALIGN(m_nWidth * 24);

	fclose(pf);

	return 0;
}

void CBmpReader::Unload() {
	if (m_buffer) {
		m_pFileHeader = NULL;
		m_pInfoHeader = NULL;
		delete[] m_buffer;
		m_buffer = NULL;
		m_pRawData = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
		m_nLineBytes = 0;
	}
}

unsigned int CBmpReader::GetWidth() const {
	return m_nWidth;
}

unsigned int CBmpReader::GetHeight() const {
	return m_nHeight;
}

PBITMAPFILEHEADER CBmpReader::GetFileHeader() const {
	return m_pFileHeader;
}

PBITMAPINFOHEADER CBmpReader::GetInfoHeader() const {
	return m_pInfoHeader;
}

char * CBmpReader::GetRawData() const {
	return m_pRawData;
}

#define GET_COLOR_AT(row, col, offset) \
	if (NULL == m_pRawData) { \
		return 0; \
	} \
	if ((row) > m_nHeight - 1 || (col) > m_nWidth - 1) { \
		return 0; \
	} \
	return m_pRawData[m_nLineBytes * (row) + 3 * (col) + (offset)];

unsigned int CBmpReader::GetRedAt(int row, int col) const {
	GET_COLOR_AT(row, col, 2);
}

unsigned int CBmpReader::GetGreenAt(int row, int col) const {
	GET_COLOR_AT(row, col, 1);
}

unsigned int CBmpReader::GetBlueAt(int row, int col) const {
	GET_COLOR_AT(row, col, 0);
}

RGB_COLOR CBmpReader::GetColorAt(int row, int col) const
{
	const RGB_COLOR ret = {
		GetRedAt(row,col),
		GetGreenAt(row,col),
		GetBlueAt(row,col)
	};
	return ret;
}

#define OUTPUT_TITLE(title) \
	cout << "[" << #title << "]" << endl;

#define OUTPUT_ITEM_CONTENT(item, name) \
	cout << "\t" << #name << ": " << item->name << endl;

void API OutputBitMapFileHeader(PBITMAPFILEHEADER pFileHeader) {
	OUTPUT_TITLE("Bitmap File Header");

	if (NULL == pFileHeader) {
		return;
	}

	OUTPUT_ITEM_CONTENT(pFileHeader, bfType);
	OUTPUT_ITEM_CONTENT(pFileHeader, bfSize);
	OUTPUT_ITEM_CONTENT(pFileHeader, bfReserved1);
	OUTPUT_ITEM_CONTENT(pFileHeader, bfReserved2);
	OUTPUT_ITEM_CONTENT(pFileHeader, bfOffBits);
}

void API OutputBitMapInfoHeader(PBITMAPINFOHEADER pInfoHeader) {
	OUTPUT_TITLE("Bitmap Info Header");

	if (NULL == pInfoHeader) {
		return;
	}

	OUTPUT_ITEM_CONTENT(pInfoHeader, biSize);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biWidth);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biHeight);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biPlanes);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biBitCount);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biCompression);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biSizeImage);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biXPelsPerMeter);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biYPelsPerMeter);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biClrUsed);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biClrImportant);
}