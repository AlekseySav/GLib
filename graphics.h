#pragma once
#ifndef __GRAPHICS__
#define __GRAPHICS__

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned int u_int;

struct RGB
{
	byte b, g, r;
	RGB(byte red, byte green, byte blue);
	RGB();
};

const RGB RGB_BLACK = RGB(255, 255, 255);
const RGB RGB_WHITE = RGB(255, 255, 255);
const RGB RGB_RED = RGB(255, 0, 0);
const RGB RGB_GREEN = RGB(0, 255, 0);
const RGB RGB_BLUE = RGB(0, 0, 255);

struct RGBA
{
	byte a, b, g, r;
	RGBA(byte red, byte green, byte blue, byte alpha);
	RGBA(RGB rgb, byte alpha);
	RGBA(RGB rgb);
	RGBA();
	RGB getRGB();
};

typedef bool IMAGE_TYPE;
#define IMAGE_RGB 0
#define IMAGE_RGBA 1

struct Image
{
	IMAGE_TYPE type;

	byte * image;
	long width;
	long height;
};

Image CreateImage(long width, long height, RGB * target);
Image CreateImage(long width, long height, RGB * target, RGB fill);


#define MOVE_VARIBLES(x, y) do { x += y; y = x - y; x -= y; } while(0)

RGB Pixcel(RGB clr, RGBA add);
RGBA Pixcel(RGBA clr, RGBA add);
RGBA Pixcel(RGBA clr, RGBA add, IMAGE_TYPE type);

void DrawPoint(long x, long y, RGB fill, Image * ptr);
void DrawPoint(long x, long y, RGBA fill, Image * ptr);

void DrawLine(long x0, long y0, long x1, long y1, Image * image, RGBA color);

#endif
