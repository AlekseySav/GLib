#include "glib_src.h"

Point glibCreatePoint(u_int x, u_int y)
{
	Point p;
	p.x = x;
	p.y = y;
	return p;
}

RGBA glibCreateRGBA(byte r, byte g, byte b, byte alpha)
{
	RGBA clr;
	clr.r = r;
	clr.g = g;
	clr.b = b;
	clr.a = alpha;
	return clr;
}

Image glibCreateImage(u_int width, u_int height)
{
	Image im = (Image)malloc(sizeof(struct Image__));
	im->width = width;
	im->height = height;

	byte * target = (byte *)malloc(width * height * 4);
	im->image = (byte *)target;

	return im;
}

void glibFillImage(Image im, RGBA fill)
{
	for (u_int y = 0; y < im->height; y++)
		for (u_int x = 0; x < im->width; x++)
			*((RGBA *)im->image + im->width * y + x) = fill;
}

RGBA glibPixel(RGBA clr, RGBA add)
{
	if (add.a == 255) return add;

	return glibCreateRGBA(
		(clr.r * clr.a + add.r * add.a) / 500,
		(clr.g * clr.a + add.g * add.a) / 500,
			(clr.b * clr.a + add.b * add.a) / 500,
			(clr.a + add.a) / 2);
}

void glibDrawPoint(Image im, Point p, RGBA fill)
{
	if (p.x >= im->width || p.x < 0 || p.y >= im->height || p.y < 0) return;

	RGBA * point = (((RGBA *)im->image) + p.y * im->width + p.x);
	RGBA clr = glibPixel(*point, fill);
	*point = clr;
}

void glibFillSquare(Image im, Point p, u_int side, RGBA color)
{
	if (side == 1)
	{
		glibDrawPoint(im, p, color);
		return;
	}

	for (u_int y = 0; y < side; y++)
		for (u_int x = 0; x < side; x++)
			glibDrawPoint(im, glibCreatePoint(p.x + x, p.y + y), color);
}

void glibFillRectangle(Image im, Point p1, Point p2, RGBA color)
{
	if (p1.x > p2.x) MOVE_VARIBLES(p1.x, p2.x);
	if (p1.y > p2.y) MOVE_VARIBLES(p1.y, p2.y);

	for (u_int y = p1.y; y < p2.y; y++)
		for (u_int x = p1.x; x < p2.x; x++)
			glibDrawPoint(im, glibCreatePoint(x, y), color);
}

void glibFillTriangle(Image im, Point p1, Point p2, Point p3, RGBA color)
{
	long a1 = p1.x - p2.x;
	long a2 = p2.x - p3.x;
	long a3 = p3.x - p1.x;

	long b1 = p2.y - p1.y;
	long b2 = p3.y - p2.y;
	long b3 = p1.y - p3.y;

	Point min = glibCreatePoint(glib_min3(p1.x, p2.x, p3.x), glib_min3(p1.y, p2.y, p3.y));
	Point max = glibCreatePoint(glib_max3(p1.x, p2.x, p3.x), glib_max3(p1.y, p2.y, p3.y));

	for (u_int x = min.x; x < max.x; x++)
		for (u_int y = min.y; y < max.y; y++)
		{
			bool a = (long)(p1.x - x) * b1 + (long)(p1.y - y) * a1 >= 0;
			bool b = (long)(p2.x - x) * b2 + (long)(p2.y - y) * a2 >= 0;
			bool c = (long)(p3.x - x) * b3 + (long)(p3.y - y) * a3 >= 0;

			if ((a && b) && c) glibDrawPoint(im, glibCreatePoint(x, y), color);
		}
}

void glibFillCircle(Image im, Point center, u_int radius, RGBA color)
{
	long r_len = radius * radius;
	for (long x = center.x - radius; x < (long)(center.x + radius); x++)
		for (long y = center.y - radius; y < (long)(center.y + radius); y++)
		{
			long x_len = x - center.x;
			long y_len = y - center.y;
			if (x_len * x_len + y_len * y_len <= r_len) glibDrawPoint(im, glibCreatePoint(x, y), color);
		}
}

void glibDrawLine(Image im, Point p1, Point p2, RGBA color, long wide)
{
	bool reversed = (glib_abs((long)(p2.x - p1.x)) < glib_abs((long)(p2.y - p1.y)));
	if (reversed)
	{
		MOVE_VARIBLES(p1.x, p1.y);
		MOVE_VARIBLES(p2.x, p2.y);
	}

	if (p1.x >= p2.x)
	{
		MOVE_VARIBLES(p1.x, p2.x);
		MOVE_VARIBLES(p1.y, p2.y);
	}

	u_int dx = p2.x - p1.x, dy = p2.y - p1.y;

	for (u_int x = p1.x; x <= p2.x; x++)
	{
		u_int y = (u_int)(dy * (x - p1.x) / dx + p1.y);
		if (reversed)
			glibFillSquare(im, glibCreatePoint(y - wide / 2, x - wide / 2), wide, color);
		else
			glibFillSquare(im, glibCreatePoint(x - wide / 2, y - wide / 2), wide, color);
	}
}

void glibDrawRectangle(Image im, Point p1, Point p2, RGBA color, long wide)
{
	glibDrawLine(im, glibCreatePoint(p1.x, p1.y), glibCreatePoint(p2.x, p1.y), color, wide);
	glibDrawLine(im, glibCreatePoint(p1.x, p1.y), glibCreatePoint(p1.x, p2.y), color, wide);
	glibDrawLine(im, glibCreatePoint(p2.x, p2.y), glibCreatePoint(p2.x, p1.y), color, wide);
	glibDrawLine(im, glibCreatePoint(p2.x, p2.y), glibCreatePoint(p1.x, p2.y), color, wide);
}

void glibDrawTriangle(Image im, Point p1, Point p2, Point p3, RGBA color, long wide)
{
	glibDrawLine(im, p1, p2, color, wide);
	glibDrawLine(im, p2, p3, color, wide);
	glibDrawLine(im, p3, p1, color, wide);
}

void glibDrawCircle(Image im, Point center, u_int radius, RGBA color, long wide)
{
	long x = 0;
	long y = radius;
	long delta = 1 - 2 * radius;
	long error = 0;
	while (y >= 0)
	{
		glibFillSquare(im, glibCreatePoint( center.x + x, center.y + y ), wide, color);
		glibFillSquare(im, glibCreatePoint(center.x + x, center.y - y), wide, color);
		glibFillSquare(im, glibCreatePoint(center.x - x, center.y + y), wide, color);
		glibFillSquare(im, glibCreatePoint(center.x - x, center.y - y), wide, color);
		error = 2 * (delta + y) - 1;
		if ((delta < 0) && (error <= 0))
		{
			delta += 2 * ++x + 1;
			continue;
		}
		if ((delta > 0) && (error > 0))
		{
			delta -= 2 * --y + 1;
			continue;
		}
		delta += 2 * (++x - y--);
	}
}

void glibFreeImage(Image im)
{
	free(im->image);
	free(im);
}
