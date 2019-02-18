#include "src/glib_src.h"

Image glibCreateImage(u_int width, u_int height)
{
	byte * image = (byte *)malloc(width * height * 4);
	return glibCreateImage(width, height, image);
}

Image glibCreateImage(u_int width, u_int height, byte * ptr)
{
	Image im = (Image)malloc(sizeof(struct Image__));
	im->width = width;
	im->height = height;
	im->image = ptr;

	return im;
}

Image glibCreateImage(Bitmap bitmap)
{
	byte * image = (byte *)malloc(bitmap->info.biHeight * bitmap->info.biWidth * 4);
	byte * ptr = image;
	byte * val = bitmap->ptr;

	for (int y = 0; y < bitmap->info.biHeight; y++)
		for (int x = 0; x < bitmap->info.biWidth; x++)
		{
			ARGB clr;
			if (bitmap->info.biBitCount == 24)
			{
				clr.a = 255;
				clr.r = val[0];
				clr.g = val[1];
				clr.b = val[2];
				val += 3;
			}
			else
			{
				clr.a = val[0];
				clr.r = val[1];
				clr.g = val[2];
				clr.b = val[3];
				val += 4;
			}

			*(ARGB *)ptr = clr;
			ptr += 4;
		}

	return glibCreateImage(bitmap->info.biWidth, bitmap->info.biHeight, image);
}

void Image__::Release()
{
	free(this->image);
	free(this);
}

void glibFillImage(Image im, ARGB color)
{
	for (int y = 0; y < im->height; y++)
		for (int x = 0; x < im->width; x++)
			*((ARGB *)im->image + im->width * y + x) = color;
}

ARGB glibMerge(ARGB clr, ARGB add)
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

ARGB * Image__::GetPixel(Point pos)
{
	return ((ARGB *)image + pos.y * width + pos.x);
}

void Image__::DrawPoint(Point point, ARGB color)
{
	if (point.x >= width || point.x < 0 || point.y >= height || point.y < 0) return;
	if (color.a == 0) return;

	ARGB * target = GetPixel(point);
	*target = glibMerge(*target, color);
}

void Image__::DrawImage(struct Image__ * image, Point start, Point end, Point sub)
{
	end.x = min(end.x, start.x + image->width);
	end.y = min(end.y, start.y + image->height);

	int32 ix = sub.x, iy = sub.y;

	for (int32 y = start.y; y < end.y; y++, iy++)
	{
		for (int32 x = start.x; x < end.x; x++, ix++)
			DrawPoint(Point{ x, y }, *image->GetPixel(Point{ ix, iy }));
		ix = sub.x;
	}
}

void Image__::DrawLine(Line line, ARGB color, long wide)
{
	bool reversed = false;
	int x1 = line.point1.x, x2 = line.point2.x, y1 = line.point1.y, y2 = line.point2.y;
	int w2 = wide / 2;

	if (_abs(y2 - y1) > _abs(x2 - x1)) {
		MOVE_VARIBLES(x1, y1);
		MOVE_VARIBLES(x2, y2);
		reversed = true;
	}

	if (x1 > x2) {
		MOVE_VARIBLES(x1, x2);
		MOVE_VARIBLES(y1, y2);
	}

	bool up = y1 - y2 >= 0;

	int dx = (x2 - x1), dy;

	int y = y1;
	if (up) dy = (y1 - y2);
	else dy = (y2 - y1);

	int err = 0;

	for (int x = x1; x < x2; x++)
	{
		err += dy;
		if (err >= dx) {
			err -= dx;
			if (up) y--;
			else y++;
		}

		if (wide == 1) {
			if (reversed) DrawPoint(Point{ y, x }, color);
			else DrawPoint(Point{ x, y }, color);
		}
		else
		{
			int mwy = y + w2;

			for (int wy = y - w2; wy < mwy; wy++) {
				if (reversed) DrawPoint(Point{ wy, x }, color);
				else DrawPoint(Point{ x, wy }, color);
			}
		}
	}
}

void Image__::DrawRect(Rect rect, ARGB color, long wide)
{
	int x1, x2, y1, y2;

	if (rect.point1.x >= rect.point2.x)
	{
		x1 = rect.point2.x;
		x2 = rect.point1.x;
	}
	else
	{
		x1 = rect.point1.x;
		x2 = rect.point2.x;
	}

	if (rect.point1.y >= rect.point2.y)
	{
		y1 = rect.point2.y;
		y2 = rect.point1.y;
	}
	else
	{
		y1 = rect.point1.y;
		y2 = rect.point2.y;
	}

	for (int y = y1; y < y2; y++)
	{
		for (int x = 0; x < wide; x++)
		{
			DrawPoint(Point{ x + x1, y }, color);
			DrawPoint(Point{ x + x2 - wide, y }, color);
		}
	}

	for (int x = x1 + wide; x < x2 - wide; x++)
	{
		for (int y = 0; y < wide; y++)
		{
			DrawPoint(Point{ x, y + y1 }, color);
			DrawPoint(Point{ x, y + y2 - wide }, color);
		}
	}
}

void Image__::DrawEllipse(struct Ellipse ellipse, ARGB color, long wide)
{
	int x0 = ellipse.center.x, y0 = ellipse.center.y;
	int rx = ellipse.radius_x, ry = ellipse.radius_y;

	int mw = rx - wide, mh = ry - wide;
	mh *= mh; mw *= mw; 
	int mhw = mh * mw;

	int hh = ry * ry, ww = rx * rx, hw = ww * hh;

	int xs = -rx;

	for(int y = 0; y < ry; y++) 
	{
		int yy = y * y;
		int x1 = xs;
		while (x1 <= 0)
		{
			if (((yy * ww) + (x1 * x1 * hh)) > hw) x1++;
			else break;
		}
		xs = x1;

		for (int x = x1; x <= 0; x++)
		{
			if (yy * mw + x * x * mh > mhw) {
				DrawPoint(Point{ x0 + x, y0 + y }, color);
				if (x) DrawPoint(Point{ x0 - x, y0 + y }, color);
				if (y) DrawPoint(Point{ x0 + x, y0 - y }, color);
				if (x && y) DrawPoint(Point{ x0 - x, y0 - y }, color);
			}
			else break;
		}
	}
}

void Image__::FillRect(Rect rect, ARGB color)
{
	int x1, x2, y1, y2;

	if (rect.point1.x >= rect.point2.x)
	{
		x1 = rect.point2.x;
		x2 = rect.point1.x;
	}
	else
	{
		x1 = rect.point1.x;
		x2 = rect.point2.x;
	}

	if (rect.point1.y >= rect.point2.y)
	{
		y1 = rect.point2.y;
		y2 = rect.point1.y;
	}
	else
	{
		y1 = rect.point1.y;
		y2 = rect.point2.y;
	}

	for (int y = y1; y < y2; y++)
		for (int x = x1; x < x2; x++)
			DrawPoint(Point{ x, y }, color);
}

void Image__::FillEllipse(struct Ellipse ellipse, ARGB color)
{
	int x0 = ellipse.center.x, y0 = ellipse.center.y;
	int rx = ellipse.radius_x, ry = ellipse.radius_y;

	int hh = ry * ry, ww = rx * rx, hw = hh * ww;

	int x00 = rx;
	int dx = 0;

	for (int x = -rx; x <= rx; x++)
		DrawPoint(Point{ x0 + x, y0 }, color);

	for (int y = 1; y <= ry; y++)
	{
		int x1 = x00 - (dx - 1);
		for (; x1 > 0; x1--)
			if (x1*x1*hh + y * y*ww <= hw)
				break;
		dx = x00 - x1;
		x00 = x1;

		for (int x = -x00; x <= x00; x++)
		{
			DrawPoint(Point{ x0 + x, y0 - y }, color);
			DrawPoint(Point{ x0 + x, y0 + y }, color);
		}
	}
}
