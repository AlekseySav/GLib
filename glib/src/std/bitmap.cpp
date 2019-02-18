#include "src/glib_src.h"

void glibCreateBMFH(BitmapFileHeader * file, byte bytes[14])
{
	file->bfType = MAKE_WORD(bytes[0], bytes[1]);
	file->bfSize = MAKE_DWORD(bytes[2], bytes[3], bytes[4], bytes[5]);
	file->bfReserved1 = MAKE_WORD(bytes[6], bytes[7]);
	file->bfReserved2 = MAKE_WORD(bytes[8], bytes[9]);
	file->bfOffBits = MAKE_DWORD(bytes[10], bytes[11], bytes[12], bytes[13]);
}

void glibFreeBMFH(BitmapFileHeader * file, byte bytes[14])
{
	FREE_WORD(file->bfType, bytes); bytes += 2;
	FREE_DWORD(file->bfSize, bytes); bytes += 4;
	FREE_WORD(file->bfReserved1, bytes); bytes += 2;
	FREE_WORD(file->bfReserved2, bytes); bytes += 2;
	FREE_DWORD(file->bfOffBits, bytes);
}

void glibCreateBMIH(BitmapInfoHeader * info, byte bytes[40])
{
	info->biSize = MAKE_DWORD(bytes[0], bytes[1], bytes[2], bytes[3]);
	info->biWidth = MAKE_DWORD(bytes[4], bytes[5], bytes[6], bytes[7]);
	info->biHeight = MAKE_DWORD(bytes[8], bytes[9], bytes[10], bytes[11]);
	info->biPlanes = MAKE_WORD(bytes[12], bytes[13]);
	info->biBitCount = MAKE_WORD(bytes[14], bytes[15]);
	info->biCompression = MAKE_DWORD(bytes[16], bytes[17], bytes[18], bytes[19]);
	info->biSizeImage = MAKE_DWORD(bytes[20], bytes[21], bytes[22], bytes[23]);
	info->biXPelsPerMeter = MAKE_DWORD(bytes[24], bytes[25], bytes[26], bytes[27]);
	info->biYPelsPerMeter = MAKE_DWORD(bytes[28], bytes[29], bytes[30], bytes[31]);
	info->biClrUsed = MAKE_DWORD(bytes[32], bytes[33], bytes[34], bytes[35]);
	info->biClrImportant = MAKE_DWORD(bytes[36], bytes[37], bytes[38], bytes[39]);
}

void glibFreeBMIH(BitmapInfoHeader * info, byte bytes[40])
{
	FREE_DWORD(info->biSize, &bytes[0]);
	FREE_DWORD((dword)info->biWidth, &bytes[4]);
	FREE_DWORD((dword)info->biHeight, &bytes[8]);
	FREE_WORD(info->biPlanes, &bytes[12]);
	FREE_WORD(info->biBitCount, &bytes[14]);
	FREE_DWORD(info->biCompression, &bytes[16]);
	FREE_DWORD(info->biSizeImage, &bytes[20]);
	FREE_DWORD((dword)info->biXPelsPerMeter, &bytes[24]);
	FREE_DWORD((dword)info->biYPelsPerMeter, &bytes[28]);
	FREE_DWORD(info->biClrUsed, &bytes[32]);
	FREE_DWORD(info->biClrImportant, &bytes[36]);
}

void Bitmap__::CreateBitmap(byte * ptr, long width, long height)
{
	long size = width * height * 4;

	this->file.bfType = FORMAT_BITMAP;
	this->file.bfSize = 54 + size;
	this->file.bfReserved1 = 0;
	this->file.bfReserved2 = 0;
	this->file.bfOffBits = 54;

	this->info.biSize = 40;
	this->info.biWidth = width;
	this->info.biHeight = height;
	this->info.biPlanes = 1;
	this->info.biBitCount = 32;
	this->info.biCompression = COMPRESSION_DEFAULT;
	this->info.biSizeImage = size;
	this->info.biXPelsPerMeter = 0;
	this->info.biYPelsPerMeter = 0;
	this->info.biClrUsed = 0;
	this->info.biClrImportant = 0;

	this->palette = NULL;

	this->ptr = (byte *)malloc(size);
	memcpy(this->ptr, ptr, size);
}

void Bitmap__::LoadFromFile(char * file, u_int32 format)
{
	if (format == FORMAT_BITMAP)
	{
		FILE * f = fopen(file, "rb");

		byte arr[54];
		fread(arr, 1, 54, f);

		glibCreateBMFH(&(this->file), arr);
		glibCreateBMIH(&(this->info), arr + 14);

		word bits = this->info.biBitCount / 8;
		long size = this->info.biWidth * this->info.biHeight * bits;

		palette = (byte *)malloc(this->file.bfOffBits - 54);
		fread(palette, 1, this->file.bfOffBits - 54, f);

		if (info.biBitCount == 32 || info.biBitCount == 24)
		{
			free(palette);
			palette = NULL;
		}

		info.biSizeImage = max(info.biSizeImage, (unsigned)size);

		byte * image = (byte *)malloc(info.biSizeImage);
		byte * tofree = image;
		fread(image, 1, info.biSizeImage, f);
		fclose(f);

		this->ptr = (byte *)malloc(size);
		byte * _ptr = this->ptr;

		int add = 4 - ((this->info.biWidth * bits) % 4); if (add == 4) add = 0;
		image += info.biSizeImage - info.biWidth * bits - add;
		int sub = 2 * info.biWidth * bits + add;
		for (int y = 0; y < info.biHeight; y++)
		{
			for (int x = 0; x < info.biWidth; x++, _ptr += bits, image += bits)
				switch (info.biBitCount)
				{
				case 0: return;
				case 1: *_ptr = *image; _ptr++; image++; break;
				case 4: *_ptr = *image; _ptr++; image++; break;
				case 8: *_ptr = *image; break;
				case 16: *(word *)_ptr = *(word *)image; _ptr++; image++; break;
				case 24:
					*(_ptr) = *(image + 2);
					*(_ptr + 1) = *(image + 1);
					*(_ptr + 2) = *(image);
					break;
				case 32:
					*(_ptr) = *(image + 3);
					*(_ptr + 1) = *(image + 2);
					*(_ptr + 2) = *(image + 1);
					*(_ptr + 3) = *(image);
					break;
				}
			image -= (sub);
		}

		free(tofree);
	}
}

void Bitmap__::Write(char * file, u_int32 format)
{
	if (format == FORMAT_BITMAP)
	{

		byte arr[54];

		glibFreeBMFH(&(this->file), arr);
		glibFreeBMIH(&(this->info), arr + 14);
		
		word bits = this->info.biBitCount / 8;
		long size = this->info.biWidth * this->info.biHeight * bits;
		
		info.biSizeImage = max(info.biSizeImage, (unsigned)size);

		byte * _ptr = (byte *)malloc(info.biSizeImage);
		byte * tofree = _ptr;

		byte * image = this->ptr;

		int add = 4 - ((this->info.biWidth * bits) % 4); if (add == 4) add = 0;
		_ptr += info.biSizeImage - info.biWidth * bits - add;
		int sub = 2 * info.biWidth * bits + add;
		for (int y = 0; y < info.biHeight; y++)
		{
			for (int x = 0; x < info.biWidth; x++, _ptr += bits, image += bits)
				switch (info.biBitCount)
				{
				case 0: return;
				case 1: *_ptr = *image; _ptr++; image++; break;
				case 4: *_ptr = *image; _ptr++; image++; break;
				case 8: *_ptr = *image; break;
				case 16: *(word *)_ptr = *(word *)image; _ptr++; image++; break;
				case 24:
					*(_ptr) = *(image + 2);
					*(_ptr + 1) = *(image + 1);
					*(_ptr + 2) = *(image);
					break;
				case 32:
					*(_ptr) = *(image + 3);
					*(_ptr + 1) = *(image + 2);
					*(_ptr + 2) = *(image + 1);
					*(_ptr + 3) = *(image);
					break;
				}
			_ptr -= (sub);
		}

		FILE * f = fopen(file, "wb");

		fwrite(arr, 1, 54, f);

		if (info.biBitCount != 32 && info.biBitCount != 24 && palette != NULL)
		{
			palette = (byte *)malloc(this->file.bfOffBits - 54);
			fwrite(palette, 1, this->file.bfOffBits - 54, f);
		}

		fwrite(tofree, 1, info.biSizeImage, f);

		fclose(f);
		free(tofree);
	}
}

void Bitmap__::Release()
{
	if (ptr != NULL) free(this->ptr);
	if (palette != NULL) free(this->palette);
	free(this);
}
