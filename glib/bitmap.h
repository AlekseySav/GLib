#pragma once
#ifndef __BITMAP__
#define __BITMAP__

word convert(byte, byte);
struct cvtw { byte a1, a2; } convert(word);

dword convert(byte, byte, byte, byte);
struct cvtd { byte a1, a2, a3, a4; } convert(dword);

struct BITMAPFILEHEADER
{
	word    bfType;
	dword   bfSize;
	word    bfReserved1;
	word    bfReserved2;
	dword   bfOffBits;
};

BITMAPFILEHEADER CreateBMFH(byte[14]);
void value(BITMAPFILEHEADER *, byte *);

struct BITMAPINFOHEADER
{
	dword  biSize;
	long   biWidth;
	long   biHeight;
	word   biPlanes;
	word   biBitCount;
	dword  biCompression;
	dword  biSizeImage;
	long   biXPelsPerMeter;
	long   biYPelsPerMeter;
	dword  biClrUsed;
	dword  biClrImportant;
};

BITMAPINFOHEADER CreateBMIH(byte[40]);
void value(BITMAPINFOHEADER *, byte *);

struct Bitmap
{
	BITMAPFILEHEADER file;
	BITMAPINFOHEADER info;
	byte * palette;
	byte * image;
};

#define BF_BITMAP 0x4D42

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

#define BITMAP_INVALID_TYPE -1
#define BITMAP_INVALID_COMPRESSION -2
#define BITMAP_INVALID_PALETTE -4
#define BITMAP_INVALID_IMAGE -8

Bitmap CreateBitmap(long width, long height, RGB * image);
Bitmap CreateBitmap(Image * image);

int WriteBitmap(const char * file, Bitmap * bitmap);
int ReadBitmap(const char * file, Bitmap * bitmap);

#ifdef __CHECKING__
#ifdef _DEBUG
#include <iostream>

void checkbmp(const char * file)
{
	FILE * f = fopen(file, "rb");
	byte bytes[__CHECKING__];
	unsigned len = fread(bytes, sizeof(char), __CHECKING__, f);
	fclose(f);

	BITMAPFILEHEADER bitmap = CreateBMFH(bytes);
	BITMAPINFOHEADER info = CreateBMIH(bytes + 14);

	std::cout << bitmap.bfType << ' ';
	std::cout << bitmap.bfSize << ' ';
	std::cout << bitmap.bfReserved1 << ' ';
	std::cout << bitmap.bfReserved2 << ' ';
	std::cout << bitmap.bfOffBits << '\n';

	std::cout << info.biSize << ' ';
	std::cout << info.biWidth << ' ';
	std::cout << info.biHeight << ' ';
	std::cout << info.biPlanes << ' ';
	std::cout << info.biBitCount << ' ';
	std::cout << info.biCompression << ' ';
	std::cout << info.biSizeImage << ' ';
	std::cout << info.biXPelsPerMeter << ' ';
	std::cout << info.biYPelsPerMeter << ' ';
	std::cout << info.biClrUsed << ' ';
	std::cout << info.biClrImportant << '\n';

	for (unsigned i = 54; i < len; i++)
	{
		char z = bytes[i];
		printf("%d ", z);
	}
}
#endif
#endif

#endif
