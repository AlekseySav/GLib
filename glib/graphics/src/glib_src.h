#pragma once
#include <glib/glib.h>

#ifdef _USES_WINDOWS_DRIVER
#undef FAILED
#include <Windows.h>

int WIN_Initialize();
int WIN_RegisterClass();
#ifdef UNICODE
int WIN_Ascii_to_UNICODE(const char * ibuf, LPWSTR obuf);
#endif

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM w, LPARAM l);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprev, LPSTR str, int cmd);

EXTERN
#ifdef UNICODE
LPCWSTR WIN_cname;
#else
char * WIN_cname;
#endif

EXTERN HINSTANCE WIN_hinstance;

#include "windows/w_window.h"
#endif
