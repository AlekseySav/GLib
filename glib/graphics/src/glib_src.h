#pragma once

#include <glib/glib.h>

#ifdef _USES_WINDOWS_DRIVER
#undef FAILED
#include <Windows.h>
#include "windows/gwindows.h"
#endif

typedef struct Window__ Window__;

EXTERN u_int windows_count;
EXTERN Window__ * last;
