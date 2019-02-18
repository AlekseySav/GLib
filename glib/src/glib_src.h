#ifndef __GLIB_SRC
#define __GLIB_SRC

#include <glib/glib.h>

#if _USER_DRIVER == _WINDOWS_DRIVER
#include "windows/windows.h"
#endif

#include "glib/std.h"

EXTERN u_int glib_min3(u_int x, u_int y, u_int z);
EXTERN u_int glib_max3(u_int x, u_int y, u_int z);

EXTERN u_int glib_windows_count;
EXTERN Window__ * glib_window_last;

#define MOVE_VARIBLES(x, y) do { x += y; y = x - y; x -= y; } while(0)
#define _abs(x) (((x) >= 0) ? (x) : (-(x)))

#endif
