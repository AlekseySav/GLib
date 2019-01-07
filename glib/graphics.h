#pragma once
#ifndef __GRAPHICS__
#define __GRAPHICS__

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned int u_int;

struct POINT
{
	long x, y;

	POINT(long x, long y);
};

struct RGB
{
	byte b, g, r;
};
RGB SetRGB(byte red, byte green, byte blue);

const RGB RGB_BLACK = SetRGB(0, 0, 0);
const RGB RGB_WHITE = SetRGB(255, 255, 255);
const RGB RGB_RED = SetRGB(255, 0, 0);
const RGB RGB_GREEN = SetRGB(0, 255, 0);
const RGB RGB_BLUE = SetRGB(0, 0, 255);

struct RGBA
{
	byte a, b, g, r;
	RGBA(byte red, byte green, byte blue, byte alpha);
	RGBA(RGB rgb, byte alpha);
	RGBA(RGB rgb);
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

	void DrawPoint(POINT p, RGB fill);
	void DrawPoint(POINT p, RGBA fill);

	void FillSquare(POINT left_upstairs, long side, RGBA clr);
	void FillRectangle(POINT p1, POINT p2, RGBA color);
	void FillTriangle(POINT p1, POINT p2, POINT p3, RGBA color);
	void FillCircle(POINT center, long radius, RGBA color);

	void DrawLine(POINT p1, POINT p2, RGBA color, long wide = 1L);
	void DrawRectangle(POINT p1, POINT p2, RGBA color, long wide = 1L);
	void DrawTriangle(POINT p1, POINT p2, POINT p3, RGBA color, long wide = 1L);
	void DrawCircle(POINT center, long radius, RGBA color, long wide = 1L);
};

Image CreateImage(long width, long height, RGB * target);
Image CreateImage(long width, long height, RGB * target, RGB fill);

#define MOVE_VARIBLES(x, y) do { x += y; y = x - y; x -= y; } while(0)

long MIN3(long x, long y, long z);
long MAX3(long x, long y, long z);

RGB Pixcel(RGB clr, RGBA add);
RGBA Pixcel(RGBA clr, RGBA add);
RGBA Pixcel(RGBA clr, RGBA add, IMAGE_TYPE type);

#endif
