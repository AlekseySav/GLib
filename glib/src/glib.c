#include "glib_src.h"

Point Mouse;

int glibLoop()
{
	return EX_MainLoop();
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
