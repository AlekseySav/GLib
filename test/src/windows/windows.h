#if !defined(_GLIB_SRC_WINDOWS) && defined(_GLIB) && _USER_DRIVER == _WINDOWS_DRIVER
#define _GLIB_SRC_WINDOWS

#include <Windows.h>
#include <io.h>
#include <fcntl.h>

EXTERN
#ifdef UNICODE
LPCWSTR WIN_cname;
#else
char * WIN_cname;
#endif

EXTERN HINSTANCE WIN_hinstance;
EXTERN bool WIN_wmpaint;

EXTERN int WIN_Initialize();
EXTERN int WIN_RegisterClass();

EXTERN int WIN_CreateWindow(Window w);
EXTERN int WIN_ShowWindow(Window w);
EXTERN int WIN_CloseWindow(Window w);
EXTERN DWORD WIN_Style(Window w);

EXTERN void WIN_Fullscreen(Window w);
EXTERN void WIN_ChangeWindowPos(Window w);
EXTERN void WIN_ChangeWindowTitle(Window w);
EXTERN void WIN_ChangeWindowFlags(Window w);
EXTERN void WIN_DrawWindow(Image im, Window w, Point min, Point max);

EXTERN Window WIN_GetWindow(void * hwnd);

EXTERN int WIN_MainLoop();

EXTERN LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM w, LPARAM l);

#ifndef __cplusplus
EXTERN int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprev, LPSTR str, int cmd);
#endif

EXTERN int WIN_Paint(EventArgs * args, Window w);
EXTERN int WIN_CreateConsole();

EXTERN void WIN_DrawText(Image im, Point pos, char * str, u_int32 format, ARGB font, ARGB back);

#endif
