#define _CRT_SECURE_NO_WARNINGS

//#define __CHECKING__ 100000

#include "graphics.h"
#include "bitmap.h"

int main(int argc, char** argv)
{
	RGB p[500][500];
	Image im = CreateImage(500, 500, *p, RGB_WHITE);

	im.FillRectangle(POINT(0, 0), POINT(100, 100), RGB_GREEN);
	im.DrawLine(POINT(100, 100), POINT(150, 300), RGB_BLUE, 10);
	im.DrawTriangle(POINT(10, 10), POINT(100, 100), POINT(100, 200), RGB_RED, 3);

	Bitmap b = CreateBitmap(im.width, im.height, (RGB *)im.image);
	WriteBitmap("1.bmp", &b);

#ifdef __CHECKING__
#ifdef _DEBUG
	checkbmp("1.bmp");
#endif
#endif
	return 0;
}
