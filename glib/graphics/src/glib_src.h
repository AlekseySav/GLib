#pragma once
#include <glib/glib.h>

#ifdef _USES_WINDOWS_DRIVER
#undef FAILED
#include <Windows.h>
#include "windows/gwindows.h"
#endif

EXTERN u_int windows_count;
