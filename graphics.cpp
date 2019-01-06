#include "graphics.h"

Image CreateImage(long width, long height, RGB * target)
{
	Image im;
	im.width = width;
	im.height = height;
	im.image = (byte *)target;
	im.type = IMAGE_RGB;
	return im;
}

Image CreateImage(long width, long height, RGB * target, RGB fill)
{
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			*(byte *)(target + width * y + x) = fill.b;
			*((byte *)(target + width * y + x) + 1) = fill.g;
			*((byte *)(target + width * y + x) + 2) = fill.r;
		}

	return CreateImage(width, height, target);
}

POINT::POINT(long x, long y)
{
	this->x = x;
	this->y = y;
}

POINT::POINT()
{ }

RGB::RGB(byte red, byte green, byte blue)
{
	r = red;
	g = green;
	b = blue;
}

RGB::RGB()
{ }

RGBA::RGBA(byte red, byte green, byte blue, byte alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

RGBA::RGBA(RGB rgb, byte alpha)
{
	r = rgb.r;
	g = rgb.g;
	b = rgb.b;
	a = alpha;
}

RGBA::RGBA(RGB rgb)
{
	r = rgb.r;
	g = rgb.g;
	b = rgb.b;
	a = 255;
}

RGBA::RGBA()
{ }

RGB RGBA::getRGB()
{
	return RGB(r, g, b);
}

RGB Pixcel(RGB clr, RGBA add)
{
	return RGB(
		(clr.r * (255 - add.a) + add.r * add.a) / 255,
		(clr.g * (255 - add.a) + add.g * add.a) / 255,
		(clr.b * (255 - add.a) + add.b * add.a) / 255);
}

RGBA Pixcel(RGBA clr, RGBA add)
{
	return RGBA(
		(clr.r * clr.a + add.r * add.a) / 500,
		(clr.g * clr.a + add.g * add.a) / 500,
		(clr.b * clr.a + add.b * add.a) / 500,
		(clr.a + add.a) / 2);
}

RGBA Pixcel(RGBA clr, RGBA add, IMAGE_TYPE type)
{
	if (type == IMAGE_RGB && add.a == 255) return add;
	if (type == IMAGE_RGB) return RGBA(Pixcel(clr.getRGB(), add), 255);
	return Pixcel(clr, add);
}

void Image::DrawPoint(POINT p, RGB fill)
{
	if (p.x >= width || p.x < 0 || p.y >= height || p.y < 0) return;

	RGB * point = ((RGB *)this->image + p.y * this->width + p.x);
	*point = fill;
	if (this->type == IMAGE_RGBA) *((byte *)point + 3) = 255;
}

void Image::DrawPoint(POINT p, RGBA fill)
{
	if (p.x >= width || p.x < 0 || p.y >= height || p.y < 0) return;

	if (this->type == IMAGE_RGB)
	{
		RGB * point = ((RGB *)this->image + p.y * this->width + p.x);
		RGB clr = Pixcel(*point, fill);
		*point = clr;
	}
	else
	{
		RGBA * point = ((RGBA *)this->image + p.y * this->width + p.x);
		RGBA clr = Pixcel(*point, fill);
		*point = clr;
	}
}

void Image::FillSquare(POINT p, long side, RGBA clr)
{
	if (side == 1)
	{
		DrawPoint(p, clr);
		return;
	}

	for (long y = 0; y < side; y++)
		for (long x = 0; x < side; x++)
			DrawPoint(POINT(p.x + x, p.y + y), clr);
}

void Image::FillRectangle(POINT p1, POINT p2, RGBA color)
{
	if (p1.x > p2.x) MOVE_VARIBLES(p1.x, p2.x);
	if (p1.y > p2.y) MOVE_VARIBLES(p1.y, p2.y);

	for (long y = p1.y; y < p2.y; y++)
		for (long x = p1.x; x < p2.x; x++)
			DrawPoint(POINT(x, y), color);
}

long abs(long x)
{
	if (x >= 0) return x;
	return -x;
}

void Image::DrawLine(POINT p1, POINT p2, RGBA color, long wide)
{
	if (p1.x >= width) p1.x = width - 1;
	if (p1.y >= height) p1.y = height - 1;
	if (p2.x >= width) p2.x = width - 1;
	if (p2.y >= height) p2.y = height - 1;

	bool reversed = (abs(p2.x - p1.x) < abs(p2.y - p1.y));
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

	long dx = p2.x - p1.x, dy = p2.y - p1.y;

	for (long x = p1.x; x <= p2.x; x++)
	{
		long y = (long)(dy * (x - p1.x) / dx + p1.y);
		if (reversed)
			this->FillSquare(POINT(y - wide / 2, x - wide / 2), wide, color);
		else
			this->FillSquare(POINT(x - wide / 2, y - wide / 2), wide, color);
	}
}

void Image::DrawRectangle(POINT p1, POINT p2, RGBA color, long wide)
{
	DrawLine(POINT(p1.x, p1.y), POINT(p2.x, p1.y), color, wide);
	DrawLine(POINT(p1.x, p1.y), POINT(p1.x, p2.y), color, wide);
	DrawLine(POINT(p2.x, p2.y), POINT(p2.x, p1.y), color, wide);
	DrawLine(POINT(p2.x, p2.y), POINT(p1.x, p2.y), color, wide);
}
