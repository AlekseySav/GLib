#include "graphics.h"

long MIN3(long x, long y, long z)
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

long MAX3(long x, long y, long z)
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

RGB SetRGB(byte red, byte green, byte blue)
{
	RGB rgb;
	rgb.r = red;
	rgb.g = green;
	rgb.b = blue;
	return rgb;
}

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

RGB RGBA::getRGB()
{
	return SetRGB(r, g, b);
}

RGB Pixcel(RGB clr, RGBA add)
{
	return SetRGB(
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

void Image::FillTriangle(POINT p1, POINT p2, POINT p3, RGBA color)
{
	long a1 = p1.x - p2.x;
	long a2 = p2.x - p3.x;
	long a3 = p3.x - p1.x;

	long b1 = p2.y - p1.y;
	long b2 = p3.y - p2.y;
	long b3 = p1.y - p3.y;

	POINT min = POINT(MIN3(p1.x, p2.x, p3.x), MIN3(p1.y, p2.y, p3.y));
	POINT max = POINT(MAX3(p1.x, p2.x, p3.x), MAX3(p1.y, p2.y, p3.y));

	for (long x = min.x; x < max.x; x++)
		for (long y = min.y; y < max.y; y++)
		{
			bool a = (p1.x - x) * b1 + (p1.y - y) * a1 >= 0;
			bool b = (p2.x - x) * b2 + (p2.y - y) * a2 >= 0;
			bool c = (p3.x - x) * b3 + (p3.y - y) * a3 >= 0;

			if (a && b && c) DrawPoint(POINT(x, y), color);
		}
}

void Image::FillCircle(POINT center, long radius, RGBA color)
{
	long r_len = radius * radius;
	for (long x = center.x - radius; x < center.x + radius; x++)
		for (long y = center.y - radius; y < center.y + radius; y++)
		{
			long x_len = x - center.x;
			long y_len = y - center.y;
			if (x_len * x_len + y_len * y_len <= r_len) DrawPoint(POINT(x, y), color);
		}
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

void Image::DrawTriangle(POINT p1, POINT p2, POINT p3, RGBA color, long wide)
{
	DrawLine(p1, p2, color, wide);
	DrawLine(p2, p3, color, wide);
	DrawLine(p3, p1, color, wide);
}

void Image::DrawCircle(POINT center, long radius, RGBA color, long wide)
{
	long x = 0;
	long y = radius;
	long delta = 1 - 2 * radius;
	long error = 0;
	while (y >= 0)
	{
		FillSquare(POINT(center.x + x, center.y + y), wide, color);
		FillSquare(POINT(center.x + x, center.y - y), wide, color);
		FillSquare(POINT(center.x - x, center.y + y), wide, color);
		FillSquare(POINT(center.x - x, center.y - y), wide, color);
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
