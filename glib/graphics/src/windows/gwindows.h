#pragma once
#ifdef _USES_WINDOWS_DRIVER

EXTERN
#ifdef UNICODE
LPCWSTR WIN_cname;
#else
char * WIN_cname;
#endif

EXTERN HINSTANCE WIN_hinstance;

#ifdef UNICODE
int WIN_Ascii_to_UNICODE(const char * ibuf, LPWSTR obuf);
#endif

int WIN_Initialize();
int WIN_RegisterClass();

int WIN_CreateWindow(Window w);
int WIN_ShowWindow(Window w);
DWORD WIN_Style(Window w);

void WIN_Fullscreen(Window w);
void WIN_ChangeWindowPos(Window w);
void WIN_ChangeWindowTitle(Window w);
void WIN_ChangeWindowFlags(Window w);

int WIN_MainLoop();

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM w, LPARAM l);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprev, LPSTR str, int cmd);

#endif
