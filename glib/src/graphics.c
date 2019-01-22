#include "glib_src.h"

Point glibCreatePoint(u_int x, u_int y)
{
	Point p;
	p.x = x;
	p.y = y;
	return p;
}

u_int glib_min3(u_int x, u_int y, u_int z)
{
	if (x < y)
	{
		if (x < z) return x;
		else return z;
	}
	else
		if (y < z) return y;
	return z;
}

u_int glib_max3(u_int x, u_int y, u_int z)
{
	if (x > y)
	{
		if (x > z) return x;
		else return z;
	}
	else
		if (y > z) return y;
	return z;
}

long glib_abs(long x)
{
	if (x >= 0) return x;
	return -x;
}

RGB glibCreateRGB(byte red, byte green, byte blue)
{
	RGB rgb;
	rgb.r = red;
	rgb.g = green;
	rgb.b = blue;
	return rgb;
}

Image glibCreateImage(u_int width, u_int height, RGB * target)
{
	Image im = (Image)malloc(sizeof(struct Image__));
	im->width = width;
	im->height = height;
	im->image = (byte *)target;
	im->type = IMAGE_RGB;
	return im;
}

Image glibFillImage(u_int width, u_int height, RGB * target, RGB fill)
{
	for (u_int y = 0; y < height; y++)
		for (u_int x = 0; x < width; x++)
			*(target + width * y + x) = fill;

	return glibCreateImage(width, height, target);
}

RGBA glibCreateRGBA(RGB rgb, byte alpha)
{
	RGBA rgba;
	rgba.r = rgb.r;
	rgba.g = rgb.g;
	rgba.b = rgb.b;
	rgba.a = alpha;

	return rgba;
}

RGB glibGetRGB(RGBA * rgba)
{
	return glibCreateRGB(rgba->r, rgba->g, rgba->b);
}

RGB glibPixelRGB(RGB clr, RGBA add)
{
	return glibCreateRGB(
		(clr.r * (255 - add.a) + add.r * add.a) / 255,
		(clr.g * (255 - add.a) + add.g * add.a) / 255,
		(clr.b * (255 - add.a) + add.b * add.a) / 255);
}

RGBA glibPixel(RGBA clr, RGBA add, u_int8 type)
{
	if (type == IMAGE_RGB && add.a == 255) return add;
	if (type == IMAGE_RGB) return glibCreateRGBA(glibPixelRGB(glibGetRGB(&clr), add), 255);
	return glibCreateRGBA(
		glibCreateRGB((clr.r * clr.a + add.r * add.a) / 500,
		(clr.g * clr.a + add.g * add.a) / 500,
			(clr.b * clr.a + add.b * add.a) / 500),
			(clr.a + add.a) / 2);
}

void glibDrawPoint(Image im, Point p, RGBA fill)
{
	if (p.x >= im->width || p.x < 0 || p.y >= im->height || p.y < 0) return;

	if (im->type == IMAGE_RGB)
	{
		RGB * point = ((RGB *)im->image + p.y * im->width + p.x);
		RGB clr = glibPixelRGB(*point, fill);
		*point = clr;
	}
	else
	{
		RGBA * point = ((RGBA *)im->image + p.y * im->width + p.x);
		RGBA clr = glibPixel(*point, fill, IMAGE_RGBA);
		*point = clr;
	}
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
			bool a = (p1.x - x) * b1 + (p1.y - y) * a1 >= 0;
			bool b = (p2.x - x) * b2 + (p2.y - y) * a2 >= 0;
			bool c = (p3.x - x) * b3 + (p3.y - y) * a3 >= 0;

			if (a && b && c) glibDrawPoint(im, glibCreatePoint(x, y), color);
		}
}

void glibFillCircle(Image im, Point center, u_int radius, RGBA color)
{
	u_int r_len = radius * radius;
	for (u_int x = center.x - radius; x < center.x + radius; x++)
		for (u_int y = center.y - radius; y < center.y + radius; y++)
		{
			u_int x_len = x - center.x;
			u_int y_len = y - center.y;
			if (x_len * x_len + y_len * y_len <= r_len) glibDrawPoint(im, glibCreatePoint(x, y), color);
		}
}

void glibDrawLine(Image im, Point p1, Point p2, RGBA color, long wide)
{
	if (p1.x >= im->width) p1.x = im->width - 1;
	if (p1.y >= im->height) p1.y = im->height - 1;
	if (p2.x >= im->width) p2.x = im->width - 1;
	if (p2.y >= im->height) p2.y = im->height - 1;

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
	u_int x = 0;
	u_int y = radius;
	u_int delta = 1 - 2 * radius;
	u_int error = 0;
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
