#include "glib_src.h"

BitmapFileHeader glibCreateBMFH(byte bytes[14])
{
	BitmapFileHeader header;
	header.bfType = MAKE_WORD(bytes[0], bytes[1]);
	header.bfSize = MAKE_DWORD(bytes[2], bytes[3], bytes[4], bytes[5]);
	header.bfReserved1 = MAKE_WORD(bytes[6], bytes[7]);
	header.bfReserved2 = MAKE_WORD(bytes[8], bytes[9]);
	header.bfOffBits = MAKE_DWORD(bytes[10], bytes[11], bytes[12], bytes[13]);

	return header;
}

void glibValueBMFH(BitmapFileHeader * h, byte * bytes)
{
	FREE_WORD(h->bfType, bytes); bytes += 2;
	FREE_DWORD(h->bfSize, bytes); bytes += 4;
	FREE_WORD(h->bfReserved1, bytes); bytes += 2;
	FREE_WORD(h->bfReserved2, bytes); bytes += 2;
	FREE_DWORD(h->bfOffBits, bytes);
}

BitmapInfoHeader glibCreateBMIH(byte bytes[40])
{
	BitmapInfoHeader header;

	header.biSize = MAKE_DWORD(bytes[0], bytes[1], bytes[2], bytes[3]);
	header.biWidth = MAKE_DWORD(bytes[4], bytes[5], bytes[6], bytes[7]);
	header.biHeight = MAKE_DWORD(bytes[8], bytes[9], bytes[10], bytes[11]);
	header.biPlanes = MAKE_WORD(bytes[12], bytes[13]);
	header.biBitCount = MAKE_WORD(bytes[14], bytes[15]);
	header.biCompression = MAKE_DWORD(bytes[16], bytes[17], bytes[18], bytes[19]);
	header.biSizeImage = MAKE_DWORD(bytes[20], bytes[21], bytes[22], bytes[23]);
	header.biXPelsPerMeter = MAKE_DWORD(bytes[24], bytes[25], bytes[26], bytes[27]);
	header.biYPelsPerMeter = MAKE_DWORD(bytes[28], bytes[29], bytes[30], bytes[31]);
	header.biClrUsed = MAKE_DWORD(bytes[32], bytes[33], bytes[34], bytes[35]);
	header.biClrImportant = MAKE_DWORD(bytes[36], bytes[37], bytes[38], bytes[39]);

	return header;
}

void glibValueBMIH(BitmapInfoHeader * h, byte * bytes)
{
	FREE_DWORD(h->biSize, &bytes[0]);
	FREE_DWORD((dword)h->biWidth, &bytes[4]);
	FREE_DWORD((dword)h->biHeight, &bytes[8]);
	FREE_WORD(h->biPlanes, &bytes[12]);
	FREE_WORD(h->biBitCount, &bytes[14]);
	FREE_DWORD(h->biCompression, &bytes[16]);
	FREE_DWORD(h->biSizeImage, &bytes[20]);
	FREE_DWORD((dword)h->biXPelsPerMeter, &bytes[24]);
	FREE_DWORD((dword)h->biYPelsPerMeter, &bytes[28]);
	FREE_DWORD(h->biClrUsed, &bytes[32]);
	FREE_DWORD(h->biClrImportant, &bytes[36]);
}

Bitmap glibCreateBitmapRGBA(long width, long height, RGBA * image, byte * ptr)
{

	Bitmap bitmap;
	bitmap.info = glibCreateBitmapInfoHeader(width, height);
	bitmap.file = glibCreateBitmapFileHeader(width, height);

	bitmap.palette = NULL;

	bitmap.image = ptr;
	for (long i = height - 1; i >= 0; i--)
		for (long j = 0; j < width; j++, ptr += 4)
		{
			*ptr = *((byte *)(image + i * width + j) + 3);
			*(ptr + 1) = *((byte *)(image + i * width + j) + 2);
			*(ptr + 2) = *((byte *)(image + i * width + j) + 1);
			*(ptr + 3) = *((byte *)(image + i * width + j));
		}
	return bitmap;
}

Bitmap glibCreateBitmap(Image image, byte * ptr)
{
	return glibCreateBitmapRGBA(image->width, image->height, (RGBA *)image->image, ptr);
}

int glibWriteBitmap(const char * file, Bitmap * bitmap)
{
	if (bitmap->file.bfType != BF_BITMAP) return BITMAP_INVALID_TYPE;
	if (bitmap->info.biCompression != BI_RGB) return BITMAP_INVALID_COMPRESSION;
	if (bitmap->info.biBitCount < 16 && bitmap->palette == NULL) return BITMAP_INVALID_PALETTE;
	if (bitmap->image == NULL) return BITMAP_INVALID_IMAGE;

	byte array[54];
	glibValueBMFH(&bitmap->file, array);
	glibValueBMIH(&bitmap->info, array + 14);

	FILE * f = fopen(file, "wb");
	fwrite(array, 1, 54, f);
	if (bitmap->palette != NULL)
		fwrite(bitmap->palette, 1,
			bitmap->file.bfSize - 54 - bitmap->info.biSizeImage, f);

	fwrite(bitmap->image, 1, bitmap->info.biSizeImage, f);
	fclose(f);
	return 0;
}

int glibReadBitmap(const char * file, Bitmap * bitmap)
{
	byte array[54];
	FILE * f = fopen(file, "rb");
	fread(array, 1, 54, f);
	bitmap->file = glibCreateBMFH(array);
	bitmap->info = glibCreateBMIH(array + 14);
	int palette_size = bitmap->file.bfOffBits - 54;
	if (palette_size < 0) return -10;
	if (palette_size == 0)
		bitmap->palette = NULL;
	else
		fread(bitmap->palette, 1, palette_size, f);
	bitmap->info.biSizeImage = 750000;
	bitmap->image = (byte *)malloc(bitmap->info.biSizeImage);

	fread(bitmap->image, 1, bitmap->info.biSizeImage, f);
	fclose(f);

	if (bitmap->file.bfType != BF_BITMAP) return BITMAP_INVALID_TYPE;
	if (bitmap->info.biCompression != BI_RGB) return BITMAP_INVALID_COMPRESSION;
	if (bitmap->info.biBitCount < 16 && bitmap->palette == NULL) return BITMAP_INVALID_PALETTE;
	if (bitmap->image == NULL) return BITMAP_INVALID_IMAGE;

	return 0;
}

BitmapFileHeader glibCreateBitmapFileHeader(long width, long height)
{
	u_int size = 4 * width * height;

	BitmapFileHeader file;

	file.bfType = BF_BITMAP;
	file.bfSize = 54 + size;
	file.bfReserved1 = 0;
	file.bfReserved2 = 0;
	file.bfOffBits = 54;

	return file;
}

BitmapInfoHeader glibCreateBitmapInfoHeader(long width, long height)
{
	BitmapInfoHeader info;

	u_int size = 4 * width * height;

	info.biSize = 40;
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;
	info.biBitCount = 32;
	info.biCompression = BI_RGB;
	info.biSizeImage = size;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	return info;
}
