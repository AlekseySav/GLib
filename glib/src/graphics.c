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

	im->image = (byte *)malloc(width * height * 4);

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
	RGBA res;
	res.a = add.a + (clr.a * (255 - add.a)) / 255;
	if (res.a == 0)
	{
		res.r = 0;
		res.g = 0;
		res.b = 0;
	}
	else
	{
		res.r = (add.r * add.a + (clr.r * clr.a * (255 - add.a)) / 255) / res.a;
		res.g = (add.g * add.a + (clr.g * clr.a * (255 - add.a)) / 255) / res.a;
		res.b = (add.b * add.a + (clr.b * clr.a * (255 - add.a)) / 255) / res.a;
	}
	return res;
}

void glibDrawPoint(Image im, Point p, RGBA fill)
{
	if (p.x >= im->width || p.x < 0 || p.y >= im->height || p.y < 0) return;

	RGBA * point = (((RGBA *)im->image) + p.y * im->width + p.x);
	RGBA clr = glibPixel(*point, fill);
	*point = clr;
}

void glibDrawStrength(Image im, Point p, u_int sizex, u_int sizey, RGBA color, bool fillax, bool fillbx, bool fillay, bool fillby, bool fillov)
{
	if (sizex == 1 && sizey == 1)
	{
		glibDrawPoint(im, p, color);
		return;
	}

	if (fillov)
	{
		for (u_int y = 0; y < sizey; y++)
			for (u_int x = 0; x < sizex; x++)
				glibDrawPoint(im, glibCreatePoint(p.x + x, p.y + y), color);
		return;
	}

	if (fillay)
		for (u_int y = 0; y < sizey; y++)
			glibDrawPoint(im, glibCreatePoint(p.x, p.y + y), color);
	if (fillax)
		for (u_int x = 0; x < sizex; x++)
			glibDrawPoint(im, glibCreatePoint(p.x + x, p.y), color);
	if (fillby)
		for (u_int y = 0; y < sizey; y++)
			glibDrawPoint(im, glibCreatePoint(p.x + sizex, p.y + y), color);
	if (fillbx)
		for (u_int x = 0; x < sizex; x++)
			glibDrawPoint(im, glibCreatePoint(p.x + x, p.y + sizey), color);
	glibDrawPoint(im, glibCreatePoint(p.x + sizex, p.y + sizey), color);
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
	bool over = false;
	bool first = true;
	long prevy = -1;

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

	if (p1.y >= p2.y) over = true;

	long dx = p2.x - p1.x, dy = p2.y - p1.y;

	for (long x = p1.x; x <= (long)p2.x; x++)
	{
		long y = (long)(dy * (long)(x - p1.x) / (long)dx + p1.y);
		if (!first)
		{
			if (over)
			{
				if (reversed)
					glibDrawStrength(im, glibCreatePoint(y - wide / 2 + 1, x - wide / 2), wide, wide, color, false, true, y != prevy, false, false);
				else
					glibDrawStrength(im, glibCreatePoint(x - wide / 2, y - wide / 2 + 1), wide, wide, color, y != prevy, false, false, true, false);
			}
			else
			{
				if (reversed)
					glibDrawStrength(im, glibCreatePoint(y - wide / 2, x - wide / 2), wide, wide, color, false, true, false, y != prevy, false);
				else
					glibDrawStrength(im, glibCreatePoint(x - wide / 2, y - wide / 2), wide, wide, color, false, y != prevy, false, true, false);
			}
		}
		else
		{
			if (reversed)
				glibDrawStrength(im, glibCreatePoint(y - wide / 2 + 1, x - wide / 2 + 1), wide, wide, color, false, false, false, false, true);
			else
				glibDrawStrength(im, glibCreatePoint(x - wide / 2 + 1, y - wide / 2 + 1), wide, wide, color, false, false, false, true, true);
			first = false;
		}
		prevy = y;
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

#define DRAW_UNDERLINE(im, point, wide, color) for(u_int i = 0; i < wide; i++) glibDrawPoint(im, glibCreatePoint(point.x + i, point.y), im)

void glibDrawCircle(Image im, Point center, u_int radius, RGBA color, long wide)
{
	if (wide == 1)
	{
		long x = 0;
		long y = radius;
		long delta = 1 - 2 * radius;
		long error = 0;
		while (y >= 0)
		{
			glibDrawPoint(im, glibCreatePoint(center.x + x, center.y + y), color);
			glibDrawPoint(im, glibCreatePoint(center.x + x, center.y - y), color);
			glibDrawPoint(im, glibCreatePoint(center.x - x, center.y + y), color);
			glibDrawPoint(im, glibCreatePoint(center.x - x, center.y - y), color);
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

	else
	{
		for (long i = (long)radius; i < (long)(wide + radius); i++)
			glibDrawCircle(im, center, (u_int)i, color, 1L);
	}
}

void glibReleaseImage(Image im)
{
	free(im->image);
	free(im);
}
