#include "src/glib_src.h"

#ifdef _USES_WINDOWS_DRIVER

DWORD WIN_Style(Window w)
{
	DWORD ws = 0;
	u_int32 style = w->flags;
	if (style & STYLE_BASIC) ws |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if (style & STYLE_FULLSCREEN) ws |= WS_POPUP;
	if (style & STYLE_MINIMIZE) ws |= WS_MINIMIZEBOX;
	if (style & STYLE_MAXIMAZE) ws |= WS_THICKFRAME | WS_MAXIMIZEBOX;
	if (style & STYLE_OVERLAPPED) ws |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	if (style & STYLE_SHOW_DISABLED) ws |= WS_DISABLED;

	return ws;
}

int WIN_CreateWindow(Window w)
{
	DWORD style = WIN_Style(w);
	RECT r;
	if (w->flags &  STYLE_FULLSCREEN)
	{
		GetWindowRect(GetDesktopWindow(), &r);
		w->x = 0;
		w->y = 0;
		w->width = r.right;
		w->height = r.bottom;
	}
	else
	{
		r.top = w->y;
		r.left = w->x;
		r.bottom = w->y + w->height;
		r.right = w->x + w->width;
		AdjustWindowRect(&r, style, FALSE);
	}
	WCHAR str[MAX_TITLE_LENGTH];

	WIN_Ascii_to_UNICODE((const char *)w->title, (LPWSTR)&str);

	w->ptr = CreateWindow(WIN_cname, (LPCWSTR)str, style, r.left, r.top, r.right - r.left, r.bottom - r.top, (HWND)(w->parent), (HMENU)NULL, WIN_hinstance, 0);
	if (!w->ptr) return -1;

	return 0;
}

int WIN_ShowWindow(Window w)
{
	if (w->flags & STYLE_SHOW_MAXIMAZE)
		ShowWindow((HWND)w->ptr, SW_MAXIMIZE);
	else if (w->flags & STYLE_SHOW_MINIMAZE)
		ShowWindow((HWND)w->ptr, SW_MINIMIZE);
	else if (w->flags & STYLE_SHOW_NOACTIVE)
		ShowWindow((HWND)w->ptr, SW_SHOWMINNOACTIVE);
	else
		ShowWindow((HWND)w->ptr, SW_SHOWDEFAULT);

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, -1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.lParam;
}

#endif
