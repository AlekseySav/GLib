#pragma once

typedef struct Point__
{
	u_int x, y;
} Point;

typedef struct ARGB__
{
	byte a, r, g, b;
} ARGB;

typedef struct Image__
{
	byte * image;
	u_int width;
	u_int height;

} * Image;

struct Line {
	Point point1, point2;
};

struct Rect {
	Point point1, point2;
};

struct Triangle {
	Point point1, point2, point3;
};

struct Ellipse {
	Point center;
	u_int radius_x, radius_y;
};

EXTERN void glibDrawPoint(Image im, Point p, ARGB fill);
EXTERN void glibDrawStrength(Image im, Point left_upstairs, u_int sizex, u_int sizey, ARGB color, bool fillax, bool fillbx, bool fillay, bool fillby, bool fillov);

EXTERN void glibDrawLine(Image im, struct Line line, ARGB color, long wide);

EXTERN void glibDrawRectangle(Image im, struct Rect rect, ARGB fill, ARGB border, long wide);
EXTERN void glibDrawTriangle(Image im, struct Triangle triangle, ARGB color, ARGB border, long wide); //border doesnt work
EXTERN void glibDrawEllipse(Image im, struct Ellipse ellipse, ARGB fill, ARGB border, long wide);

EXTERN Image glibCreateImage(u_int width, u_int height);
EXTERN void glibReleaseImage(Image im);
EXTERN void glibFillImage(Image im, ARGB fill);

#define MOVE_VARIBLES(x, y) do { x += y; y = x - y; x -= y; } while(0)

EXTERN ARGB glibPixel(ARGB clr, ARGB add);
