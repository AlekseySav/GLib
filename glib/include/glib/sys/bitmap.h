#if !defined(_GLIB_SYS_BITMAP_) && defined(_GLIB)
#define _GLIB_SYS_BITMAP_

struct BitmapFileHeader {
	word    bfType;
	dword   bfSize;
	word    bfReserved1;
	word    bfReserved2;
	dword   bfOffBits;
};

struct BitmapInfoHeader {
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


EXTERN void glibCreateBMFH(BitmapFileHeader * file, byte bytes[14]);
EXTERN void glibFreeBMFH(BitmapFileHeader * file, byte bytes[14]);

EXTERN void glibCreateBMIH(BitmapInfoHeader * info, byte bytes[40]);
EXTERN void glibFreeBMIH(BitmapInfoHeader * info, byte bytes[40]);

#define FORMAT_BITMAP 0x4d42
//#define FORMAT_JPEG	 0

#define COMPRESSION_DEFAULT    0
#define COMPRESSION_RLE8       1
#define COMPRESSION_RLE4       2
#define COMPRESSION_BITFIELDS  3
#define COMPRESSION_JPEG       4
#define COMPRESSION_PNG        5

typedef struct Bitmap__ {
	BitmapFileHeader file;
	BitmapInfoHeader info;
	byte * palette;
	byte * ptr;
	
	void CreateBitmap(byte * ptr, long width, long height);

	void LoadFromFile(char * file, u_int32 format = FORMAT_BITMAP);
	void Write(char * file, u_int32 format = FORMAT_BITMAP);

	void Release();
} * Bitmap;

#define glibBitmap() ((Bitmap)(Bitmap__ *)malloc(sizeof(struct Bitmap__)))

#endif
