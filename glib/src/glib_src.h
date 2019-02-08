#pragma once

#include <glib/glib.h>

#ifdef _USES_WINDOWS_DRIVER
#undef FAILED
#include <Windows.h>
#include "windows/gwindows.h"
#endif

#ifndef __GLIB_SRC
#define __GLIB_SRC
#endif

#include "glib/std.h"

typedef struct Window__ Window__;

EXTERN u_int glib_min3(u_int x, u_int y, u_int z);
EXTERN u_int glib_max3(u_int x, u_int y, u_int z);
EXTERN long glib_abs(long x);

EXTERN u_int glib_windows_count;
EXTERN Window__ * glib_window_last;

EXTERN u_int32 glib_language;
