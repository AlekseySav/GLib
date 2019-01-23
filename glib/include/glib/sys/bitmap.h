#pragma once

typedef struct BITMAPFILEHEADER__
{
	word    bfType;
	dword   bfSize;
	word    bfReserved1;
	word    bfReserved2;
	dword   bfOffBits;
} BitmapFileHeader;

EXTERN BitmapFileHeader glibCreateBMFH(byte bytes[14]);
EXTERN void glibValueBMFH(BitmapFileHeader * h, byte * bytes);

typedef struct BITMAPINFOHEADER__
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
} BitmapInfoHeader;

EXTERN BitmapInfoHeader glibCreateBMIH(byte bytes[40]);
EXTERN void glibValueBMIH(BitmapInfoHeader * h, byte * bytes);

typedef struct Bitmap__
{
	BitmapFileHeader file;
	BitmapInfoHeader info;
	byte * palette;
	byte * image;
} Bitmap;

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

EXTERN Bitmap glibCreateBitmapRGB(long width, long height, RGB * image);
EXTERN Bitmap glibCreateBitmapRGBA(long width, long height, RGBA * image);
EXTERN Bitmap glibCreateBitmap(Image image);

EXTERN int glibWriteBitmap(const char * file, Bitmap * bitmap);
EXTERN int glibReadBitmap(const char * file, Bitmap * bitmap);
 