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
	RGBA(rgb, 255);
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

void DrawPoint(long x, long y, RGB fill, Image * ptr)
{
	RGB * point = ((RGB *)ptr->image + y * ptr->width + x);
	*point = fill;
	if (ptr->type == IMAGE_RGBA) *((byte *)point + 3) = 255;
}

void DrawPoint(long x, long y, RGBA fill, Image * ptr)
{
	if (ptr->type == IMAGE_RGB)
	{
		RGB * point = ((RGB *)ptr->image + y * ptr->width + x);
		RGB clr = Pixcel(*point, fill);
		*point = clr;
	}
	else
	{
		RGBA * point = ((RGBA *)ptr->image + y * ptr->width + x);
		RGBA clr = Pixcel(*point, fill);
		*point = clr;
	}
}

void DrawLine(long x0, long y0, long x1, long y1, Image * image, RGBA color)
{
	bool reversed = (x1 - x0 < y1 - y0);
	if (reversed)
	{
		MOVE_VARIBLES(x0, y0);
		MOVE_VARIBLES(x1, y1);
	}

	if (x0 >= x1)
	{
		MOVE_VARIBLES(x0, x1);
		MOVE_VARIBLES(y0, y1);
	}

	float alpha = (float)(y1 - y0) / (x1 - x0);

	for (int x = x0; x < x1; x++)
	{
		int y = (int)(alpha * (x - x0) + y0 - 1);
		if (reversed)
			DrawPoint(y, x, color, image);
		else
			DrawPoint(x, y, color, image);
	}
}
