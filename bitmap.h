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



#ifdef _DEBUG
#include <iostream>

void checkbmp(const char * file);
#endif

#endif
