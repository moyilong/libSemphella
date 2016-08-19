#include "libQr.h"
#include "qrencode.h"


void CodeToBMP(QRcode *core, unsigned char *dbuff, uint64_t &len, uint64_t prescale, const unsigned char PIXEL_COLOR_G=0x00, const unsigned char PIXEL_COLOR_R=0x00, const unsigned char PIXEL_COLOR_B=0xff)
{
	uint64_t wx;
	wx = core->width;
	uint64_t wxAdj = wx*prescale * 3;
	if (wxAdj % 4)
		wxAdj = (wxAdj / 4 + 1) * 4;
	uint64_t undata = wxAdj*wx*prescale;
	unsigned char *buff = (unsigned char *)malloc(undata);
	len = undata;
	memset(buff, 0, sizeof(buff));
	BITMAPFILEHEADER bmp;
	bmp.bfType = 0x4d42;
	bmp.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + undata;
	bmp.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	BITMAPINFOHEADER info;
	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = wx * prescale;
	info.biHeight = -((int)wx * prescale);
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant = 0;


	unsigned char *pSourceData = core->data;
	uint64_t y, x, n, l;
	for (y = 0; y < wx; y++)
	{
		unsigned char *pDestData = buff + wxAdj * y * prescale;
		for (x = 0; x < wx; x++)
		{
			if (*pSourceData & 1)
			{
				for (l = 0; l < prescale; l++)
				{
					for (n = 0; n < prescale; n++)
					{
						*(pDestData + n * 3 + wx * l) = PIXEL_COLOR_B;
						*(pDestData + 1 + n * 3 + wxAdj * l) = PIXEL_COLOR_G;
						*(pDestData + 2 + n * 3 + wxAdj * l) = PIXEL_COLOR_R;
					}
				}
			}
			pDestData += 3 * prescale;
			pSourceData++;
		}
	}
	debug << "Allocing End Mem..." << endl;
	buff = (unsigned char *)malloc(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + undata);
	debug << "Copt 1/3" << endl;
	memcpy(buff, &bmp, sizeof(BITMAPFILEHEADER));
	debug << "Copy 2/3" << endl;
	memcpy(buff + sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	debug << "Copy 3/3" << endl;
	memcpy(buff + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), dbuff, undata);
	debug << "Final! Freeing..." << endl;
}


QRAPI void CreateBMPBUff(string data, char *ptr, uint64_t &len,uint64_t prescale)
{
	QRcode *core = QRcode_encodeString(data.data(), 0, QR_ECLEVEL_H, QR_MODE_8, 1);
 	CodeToBMP(core, (unsigned char*)ptr, len, prescale);
}

QRAPI void CreateDataBuff(const char *data,uint64_t dlen ,char *ptr, uint64_t &len, uint64_t prescale)
{
	QRcode *core = QRcode_encodeData(dlen, (const unsigned char*)data, 0, QR_ECLEVEL_H);
	CodeToBMP(core, (unsigned char*)ptr, len, prescale);
}
