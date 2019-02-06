#include "glib_src.h"

Image glibCreateImage(u_int width, u_int height)
{
	Image im = (Image)malloc(sizeof(struct Image__));
	im->width = width;
	im->height = height;

	im->image = (byte *)malloc(width * height * 4);

	return im;
}

void glibFillImage(Image im, ARGB fill)
{
	for (u_int y = 0; y < im->height; y++)
		for (u_int x = 0; x < im->width; x++)
			*((ARGB *)im->image + im->width * y + x) = fill;
}

ARGB glibPixel(ARGB clr, ARGB add)
{
	ARGB res;
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

void glibDrawPoint(Image im, Point p, ARGB fill)
{
	if (p.x >= im->width || p.x < 0 || p.y >= im->height || p.y < 0) return;
	if (fill.a == 0) return;

	ARGB * point = (((ARGB *)im->image) + p.y * im->width + p.x);
	ARGB clr = glibPixel(*point, fill);
	*point = clr;
}

void glibDrawStrength(Image im, Point p, u_int sizex, u_int sizey, ARGB color, bool fillax, bool fillbx, bool fillay, bool fillby, bool fillov)
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
				glibDrawPoint(im, (Point){ p.x + x, p.y + y }, color);
		return;
	}

	if (fillay)
		for (u_int y = 0; y < sizey; y++)
			glibDrawPoint(im, (Point){ p.x, p.y + y }, color);
	if (fillax)
		for (u_int x = 0; x < sizex; x++)
			glibDrawPoint(im, (Point) { p.x + x, p.y }, color);
	if (fillby)
		for (u_int y = 0; y < sizey; y++)
			glibDrawPoint(im, (Point) {	p.x + sizex, p.y + y }, color);
	if (fillbx)
		for (u_int x = 0; x < sizex; x++)
			glibDrawPoint(im, (Point) { p.x + x, p.y + sizey }, color);
	glibDrawPoint(im, (Point) { p.x + sizex, p.y + sizey }, color);
}

void glibDrawRectangle(Image im, struct Rect rect, ARGB fill, ARGB border, long wide)
{
	Point p1 = rect.point1, p2 = rect.point2;

	if (p1.x > p2.x) MOVE_VARIBLES(p1.x, p2.x);
	if (p1.y > p2.y) MOVE_VARIBLES(p1.y, p2.y);

	for (u_int y = p1.y; y < p2.y; y++)
		for (u_int x = p1.x; x < p2.x; x++)
		{
			if(y <= wide + p1.y || y >= p2.y - wide || x <= wide + p1.x || x >= p2.x - wide)
				glibDrawPoint(im, (Point) { x, y }, border);
			else
				glibDrawPoint(im, (Point) { x, y }, fill);
		}
}

void glibDrawTriangle(Image im, struct Triangle triangle, ARGB color, ARGB border, long wide)
{
	//Point p1 = triangle.point1, p2 = triangle.point2, p3 = triangle.point3;

	//long a1 = p1.x - p2.x;
	//long a2 = p2.x - p3.x;
	//long a3 = p3.x - p1.x;

	//long b1 = p2.y - p1.y;
	//long b2 = p3.y - p2.y;
	//long b3 = p1.y - p3.y;

	////long l12 = p1.x + p1.y - p2.x - p2.y;

	//Point min = { glib_min3(p1.x, p2.x, p3.x), glib_min3(p1.y, p2.y, p3.y) };
	//Point max = { glib_max3(p1.x, p2.x, p3.x), glib_max3(p1.y, p2.y, p3.y) };

	//for (u_int x = min.x; x < max.x; x++)
	//	for (u_int y = min.y; y < max.y; y++)
	//	{
	//		long a = (long)(p1.x - x) * b1 + (long)(p1.y - y) * a1;
	//		long b = (long)(p2.x - x) * b2 + (long)(p2.y - y) * a2;
	//		long c = (long)(p3.x - x) * b3 + (long)(p3.y - y) * a3;

	//		if (a >= 0 && b >= 0 && c >= 0 || a <= 0 && b <= 0 && c <= 0)
	//		{
	//			//if(a + wide >= l12 || b + wide >= 0 || c + wide >= -200)
	//				glibDrawPoint(im, (Point)(x, y), color);
	//		}
	//	}
}

void glibDrawEllipse(Image im, struct Ellipse ellipse, ARGB fill, ARGB border, long wide)
{
	long lx = ellipse.radius_x * ellipse.radius_x, ly = ellipse.radius_y * ellipse.radius_y, l = lx * ly;
	long mlx = (ellipse.radius_x - wide) * (ellipse.radius_x - wide), mly = (ellipse.radius_y - wide) * (ellipse.radius_y - wide), ml = lx * ly;

	for (long y = ellipse.center.y - ellipse.radius_y; y < (long)ellipse.center.y + ellipse.radius_y; y++)
	{
		long yy = (y - ellipse.center.y);
		for (long x = ellipse.center.x - ellipse.radius_x; x < (long)ellipse.center.x + ellipse.radius_x; x++)
		{
			long xx = (x - ellipse.center.x);
			
			if (xx * xx * ly + yy * yy * lx >= l) glibDrawPoint(im, (Point) { x, y }, border);
			if (xx * xx * mly + yy * yy * mlx <= ml) glibDrawPoint(im, (Point) { x, y }, fill);
		}
	}
}

void glibDrawLine(Image im, struct Line line, ARGB color, long wide)
{
	Point p1 = line.point1, p2 = line.point2;

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
					glibDrawStrength(im, (Point) { y - wide / 2 + 1, x - wide / 2 }, wide, wide, color, false, true, y != prevy, false, false);
				else
					glibDrawStrength(im, (Point) { x - wide / 2, y - wide / 2 + 1 }, wide, wide, color, y != prevy, false, false, true, false);
			}
			else
			{
				if (reversed)
					glibDrawStrength(im, (Point) { y - wide / 2, x - wide / 2 }, wide, wide, color, false, true, false, y != prevy, false);
				else
					glibDrawStrength(im, (Point) { x - wide / 2, y - wide / 2 }, wide, wide, color, false, y != prevy, false, true, false);
			}
		}
		else
		{
			if (reversed)
				glibDrawStrength(im, (Point) { y - wide / 2 + 1, x - wide / 2 + 1 }, wide, wide, color, false, false, false, false, true);
			else
				glibDrawStrength(im, (Point) { x - wide / 2 + 1, y - wide / 2 + 1 }, wide, wide, color, false, false, false, true, true);
			first = false;
		}
		prevy = y;
	}
}

void glibReleaseImage(Image im)
{
	free(im->image);
	free(im);
}
