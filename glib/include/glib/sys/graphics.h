#pragma once

typedef struct Point__
{
	u_int x, y;
} Point;

EXTERN Point glibCreatePoint(u_int x, u_int y);

typedef struct RGB__
{
	byte r, g, b;
} RGB;

EXTERN RGB glibCreateRGB(byte red, byte green, byte blue);

#define RGB_BLACK glibCreateRGB(255, 255, 255)
#define RGB_WHITE glibCreateRGB(255, 255, 255)
#define RGB_RED glibCreateRGB(255, 255, 255)
#define RGB_GREEN glibCreateRGB(255, 255, 255)
#define RGB_BLUE glibCreateRGB(255, 255, 255)

typedef struct RGBA__
{
	byte a, r, g, b;
} RGBA;

EXTERN RGBA glibCreateRGBA(RGB rgb, byte alpha);
EXTERN RGB glibGetRGB(RGBA * rgba);

#define IMAGE_RGB 0
#define IMAGE_RGBA 1

typedef struct Image__
{
	byte type;

	byte * image;
	u_int width;
	u_int height;

} * Image;

EXTERN void glibDrawPoint(Image im, Point p, RGBA fill);

EXTERN void glibFillSquare(Image im, Point left_upstairs, u_int side, RGBA clr);
EXTERN void glibFillRectangle(Image im, Point p1, Point p2, RGBA color);
EXTERN void glibFillTriangle(Image im, Point p1, Point p2, Point p3, RGBA color);
EXTERN void glibFillCircle(Image im, Point center, u_int radius, RGBA color);

EXTERN void glibDrawLine(Image im, Point p1, Point p2, RGBA color, long wide);
EXTERN void glibDrawRectangle(Image im, Point p1, Point p2, RGBA color, long wide);
EXTERN void glibDrawTriangle(Image im, Point p1, Point p2, Point p3, RGBA color, long wide);
EXTERN void glibDrawCircle(Image im, Point center, u_int radius, RGBA color, long wide);

EXTERN Image glibCreateImage(u_int width, u_int height, RGB * target);
EXTERN Image glibFillImage(u_int width, u_int height, RGB * target, RGB fill);

#define MOVE_VARIBLES(x, y) do { x += y; y = x - y; x -= y; } while(0)

EXTERN RGB glibPixelRGB(RGB clr, RGBA add);

EXTERN RGBA glibPixel(RGBA clr, RGBA add, u_int8 type);
