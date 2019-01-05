#define _CRT_SECURE_NO_WARNINGS

#include "graphics.h"
#include "bitmap.h"


int main(int argc, char** argv) 
{
	RGB p[100][100];

	Image im = CreateImage(100, 100, p[0], RGB_WHITE);
	DrawLine(0, 0, 100, 100, &im, RGBA(RGB_RED, 50));
	DrawLine(100, 0, 0, 100, &im, RGBA(RGB_BLUE));

	Bitmap b = CreateBitmap(im.width, im.height, (RGB *)im.image);
	WriteBitmap("1.bmp", &b);

#ifdef _DEBUG
	checkbmp("1.bmp");
	printf("\n\n");
	checkbmp("new.bmp");
#endif
	return 0;
}
