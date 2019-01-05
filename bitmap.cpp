#define _CRT_SECURE_NO_WARNINGS

#include "graphics.h"
#include "bitmap.h"
#include <stdio.h>

word convert(byte a1, byte a2)
{
	union converter
	{
		word w;
		struct { byte a, b; } b;
	};
	converter t; t.b = { a1, a2 };
	return t.w;
}

cvtw convert(word w)
{
	union converter
	{
		word w;
		cvtw b;
	};
	converter t; t.w = w;
	return t.b;
}

dword convert(byte a1, byte a2, byte a3, byte a4)
{
	union converter
	{
		dword w;
		struct { byte a, b, c, d; } b;
	};
	converter t; t.b = { a1, a2, a3, a4 };
	return t.w;
}

struct cvtd convert(dword w)
{
	union converter
	{
		dword w;
		cvtd b;
	};
	converter t; t.w = w;
	return t.b;
}

BITMAPFILEHEADER CreateBMFH(byte bytes[14])
{
	BITMAPFILEHEADER header;
	header.bfType = convert(bytes[0], bytes[1]);
	header.bfSize = convert(bytes[2], bytes[3], bytes[4], bytes[5]);
	header.bfReserved1 = convert(bytes[6], bytes[7]);
	header.bfReserved2 = convert(bytes[8], bytes[9]);
	header.bfOffBits = convert(bytes[10], bytes[11], bytes[12], bytes[13]);

	return header;
}

void value(BITMAPFILEHEADER * h, byte * bytes)
{
	cvtw s = convert(h->bfType); 
	bytes[0] = s.a1; bytes[1] = s.a2;
	cvtd d = convert(h->bfSize);
	bytes[2] = d.a1; bytes[3] = d.a2; bytes[4] = d.a3; bytes[5] = d.a4;
	s = convert(h->bfReserved1);
	bytes[6] = s.a1; bytes[7] = s.a2;
	s = convert(h->bfReserved2);
	bytes[8] = s.a1; bytes[9] = s.a2;
	d = convert(h->bfOffBits);
	bytes[10] = d.a1; bytes[11] = d.a2; bytes[12] = d.a3; bytes[13] = d.a4;
}

BITMAPINFOHEADER CreateBMIH(byte bytes[40])
{
	BITMAPINFOHEADER header;
	header.biSize = convert(bytes[0], bytes[1], bytes[2], bytes[3]);
	header.biWidth = convert(bytes[4], bytes[5], bytes[6], bytes[7]);
	header.biHeight = convert(bytes[8], bytes[9], bytes[10], bytes[11]);
	header.biPlanes = convert(bytes[12], bytes[13]);
	header.biBitCount = convert(bytes[14], bytes[15]);
	header.biCompression = convert(bytes[16], bytes[17], bytes[18], bytes[19]);
	header.biSizeImage = convert(bytes[20], bytes[21], bytes[22], bytes[23]);
	header.biXPelsPerMeter = convert(bytes[24], bytes[25], bytes[26], bytes[27]);
	header.biYPelsPerMeter = convert(bytes[28], bytes[29], bytes[30], bytes[31]);
	header.biClrUsed = convert(bytes[32], bytes[33], bytes[34], bytes[35]);
	header.biClrImportant = convert(bytes[36], bytes[37], bytes[38], bytes[39]);

	return header;
}


void __bihvalue(word w, byte * b)
{
	cvtw c = convert(w);
	*b = c.a1;
	*(b + 1) = c.a2;
}

void __bihvalue(dword w, byte * b)
{
	cvtd c = convert(w);
	*b = c.a1;
	*(b + 1) = c.a2;
	*(b + 2) = c.a3;
	*(b + 3) = c.a4;
}

void value(BITMAPINFOHEADER * h, byte * bytes)
{
	__bihvalue(h->biSize, &bytes[0]);
	__bihvalue((dword)h->biWidth, &bytes[4]);
	__bihvalue((dword)h->biHeight, &bytes[8]);
	__bihvalue(h->biPlanes, &bytes[12]);
	__bihvalue(h->biBitCount, &bytes[14]);
	__bihvalue(h->biCompression, &bytes[16]);
	__bihvalue(h->biSizeImage, &bytes[20]);
	__bihvalue((dword)h->biXPelsPerMeter, &bytes[24]);
	__bihvalue((dword)h->biYPelsPerMeter, &bytes[28]);
	__bihvalue(h->biClrUsed, &bytes[32]);
	__bihvalue(h->biClrImportant, &bytes[36]);
}

Bitmap CreateBitmap(long width, long height, RGB * image)
{
	Bitmap bitmap;

	bitmap.info.biSize = 40;
	bitmap.info.biWidth = width;
	bitmap.info.biHeight = height;
	bitmap.info.biPlanes = 1;
	bitmap.info.biBitCount = 24;
	bitmap.info.biCompression = BI_RGB;
	bitmap.info.biSizeImage = (width * 3 + width % 4) * height;
	bitmap.info.biXPelsPerMeter = 0;
	bitmap.info.biYPelsPerMeter = 0;
	bitmap.info.biClrUsed = 0;
	bitmap.info.biClrImportant = 0;

	bitmap.file.bfType = BF_BITMAP;
	bitmap.file.bfSize = 54 + bitmap.info.biSizeImage;
	bitmap.file.bfReserved1 = 0;
	bitmap.file.bfReserved2 = 0;
	bitmap.file.bfOffBits = 54; 
	
	bitmap.palette = nullptr;
	bitmap.image = (byte *)image;
	return bitmap;
}

Bitmap CreateBitmap(Image * image)
{
	if (image->type = IMAGE_RGB)
		return CreateBitmap(image->width, image->height, (RGB *)image->image);
	else
		return { 0 };
}

int WriteBitmap(const char * file, Bitmap * bitmap)
{
	if (bitmap->file.bfType != BF_BITMAP) return BITMAP_INVALID_TYPE;
	if (bitmap->info.biCompression != BI_RGB) return BITMAP_INVALID_COMPRESSION;
	if (bitmap->info.biBitCount < 16 && bitmap->palette == nullptr) return BITMAP_INVALID_PALETTE;
	if (bitmap->image == nullptr) return BITMAP_INVALID_IMAGE;

	byte array[54];
	value(&bitmap->file, array);
	value(&bitmap->info, array + 14);

	FILE * f = fopen(file, "wb");
	fwrite(array, 1, 54, f);
	if (bitmap->palette != nullptr)
		fwrite(bitmap->palette, 1,
			bitmap->file.bfSize - 54 - bitmap->info.biSizeImage, f);
	byte * ptr = bitmap->image + bitmap->info.biSizeImage - 1;
	byte imline = 0;
	for (int i = bitmap->info.biHeight - 1; i >= 0 ; i--)
	{
		fwrite(bitmap->image + i * bitmap->info.biWidth * bitmap->info.biBitCount / 8, 1, bitmap->info.biWidth * bitmap->info.biBitCount / 8, f);
		int t = bitmap->info.biWidth % 4;
		while (t--) fwrite(&imline, 1, 1, f);
	}

	fclose(f);
	return 0;
}

int ReadBitmap(const char * file, Bitmap * bitmap)
{
	return 0;
}

#ifdef _DEBUG
void checkbmp(const char * file)
{
	FILE * f = fopen(file, "rb");
	byte bytes[256];
	unsigned len = fread(bytes, sizeof(char), 256, f);
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
