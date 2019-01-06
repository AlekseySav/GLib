#define _CRT_SECURE_NO_WARNINGS

#include "graphics.h"
#include "bitmap.h"

int main(int argc, char** argv) 
{
	RGB p[500][500];

	Image im = CreateImage(500, 500, *p, RGB_WHITE);

	im.FillRectangle(POINT(0, 0), POINT(500, 500), RGB_GREEN);

	Bitmap b = CreateBitmap(im.width, im.height, (RGB *)im.image);
	WriteBitmap("1.bmp", &b);

#ifdef __CHECKING__
	#ifdef _DEBUG
		checkbmp("1.bmp");
	#endif
#endif
	return 0;
}
