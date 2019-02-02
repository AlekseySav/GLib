#pragma once
#ifdef _USES_WINDOWS_DRIVER

EXTERN
#ifdef UNICODE
LPCWSTR WIN_cname;
#else
char * WIN_cname;
#endif

EXTERN HINSTANCE WIN_hinstance;
EXTERN bool WIN_wmpaint;

#ifdef UNICODE
EXTERN int WIN_Ascii_to_UNICODE(const char * ibuf, LPWSTR obuf);
#endif

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
EXTERN void WIN_DrawWindow(Image im, Window w);

EXTERN Window WIN_GetWindow(void * hwnd);

EXTERN int WIN_MainLoop();

EXTERN LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM w, LPARAM l);
EXTERN int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprev, LPSTR str, int cmd);

EXTERN int WIN_Paint(EventArgs * args, Window w);

#endif
