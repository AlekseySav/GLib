#include "src/glib_src.h"

#if _USER_DRIVER == _WINDOWS_DRIVER

void WIN_Fullscreen(Window w)
{
	RECT r;
	GetWindowRect(GetDesktopWindow(), &r);
	w->x = 0;
	w->y = 0;
	w->width = r.right;
	w->height = r.bottom;
}

DWORD WIN_Style(Window w)
{
	DWORD ws = 0;
	u_int32 style = w->flags;
	if (style & STYLE_BASIC) ws |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if ((style & STYLE_FULLSCREEN) || (style & STYLE_NOBORDER)) ws |= WS_POPUP;
	if (style & STYLE_MINIMIZEABLE) ws |= WS_MINIMIZEBOX;
	if (style & STYLE_MAXIMIZEABLE) ws |= WS_MAXIMIZEBOX;
	if (style & STYLE_OVERLAPPED) ws |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	if (style & STATE_DISABLED) ws |= WS_DISABLED;
	if (style & STYLE_THICKFRAME) ws |= WS_THICKFRAME;

	if (style & STATE_MAXIMIZE) ws |= WS_MAXIMIZE;
	if (style & STATE_MINIMIZE) ws |= WS_MINIMIZE;

	return ws;
}

int WIN_CreateWindow(Window w)
{
	DWORD style = WIN_Style(w);

#ifdef UNICODE
	WCHAR str[MAX_TITLE_LENGTH];
	glibConvertString(w->title, &str[0]);
#else
	char * str = w->title;
#endif

	if (w->flags &  STYLE_FULLSCREEN)
	{
		WIN_Fullscreen(w);
		w->ptr = CreateWindow(WIN_cname, (LPCWSTR)str, style, 0, 0, w->width, w->height, (HWND)(w->parent), (HMENU)NULL, WIN_hinstance, 0);
	}
	else
	{
		if (w->x != WINDOW_DEFAULT_POSITION)
		{
			RECT r;
			r.top = w->y;
			r.left = w->x;
			r.bottom = w->y + w->height;
			r.right = w->x + w->width;
			AdjustWindowRect(&r, style, FALSE);

			long dy = w->y - r.top;
			long dx = w->x - r.left;

			r.top += dy; r.bottom += dy;
			r.left += dx; r.right += dx;

			w->ptr = CreateWindow(WIN_cname, (LPCWSTR)str, style, r.left, r.top, r.right - r.left, r.bottom - r.top,
				(HWND)(w->parent), (HMENU)NULL, WIN_hinstance, 0);
		}
		else
			w->ptr = CreateWindow(WIN_cname, (LPCWSTR)str, style, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
			(HWND)(w->parent), (HMENU)NULL, WIN_hinstance, 0);
	}
	if (!w->ptr) return -1;

	return 0;
}

int WIN_ShowWindow(Window w)
{
	if (!(w->flags & SYS_CREATED)) return -1;

	if (w->flags & STATE_MAXIMIZE)
		ShowWindow((HWND)w->ptr, SW_MAXIMIZE);
	else if (w->flags & STATE_MINIMIZE)
		ShowWindow((HWND)w->ptr, SW_MINIMIZE);
	else if (w->flags & SHOW_NOACTIVE)
		ShowWindow((HWND)w->ptr, SW_SHOWMINNOACTIVE);
	else
		ShowWindow((HWND)w->ptr, SW_SHOWDEFAULT);

	return 0;
}

int WIN_CloseWindow(Window w)
{
	DestroyWindow((HWND)w->ptr);
	if (glib_windows_count == 0) PostQuitMessage(0);
	return 0;
}

void WIN_ChangeWindowPos(Window w)
{
	RECT r = { (LONG)w->x, (LONG)w->y, (LONG)w->x + (LONG)w->width, (LONG)w->y + (LONG)w->height };
	AdjustWindowRect(&r, WIN_Style(w), FALSE);
	SetWindowPos((HWND)w->ptr, HWND_TOP, r.left, r.top, r.right, r.bottom, 0);

}

void WIN_ChangeWindowTitle(Window w)
{
#ifdef UNICODE
	WCHAR str[MAX_TITLE_LENGTH];
	glibConvertString(w->title, &str[0]);
#else
	char * str = w->title;
#endif

	SetWindowText((HWND)w->ptr, str);
}

void WIN_ChangeWindowFlags(Window w)
{
	if (!(w->flags & SYS_CREATED)) return;

	SetWindowLongPtr((HWND)w->ptr, GWL_STYLE, WIN_Style(w));
	SetWindowPos((HWND)w->ptr, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	if (w->flags & STYLE_FULLSCREEN)
	{
		WIN_Fullscreen(w);
		SetWindowPos((HWND)w->ptr, HWND_BOTTOM, 0, 0, w->width, w->height, 0);
	}
}

Window WIN_GetWindow(void * hwnd)
{
	if (glib_window_last == NULL) return NULL;

	Window w = glib_window_last;
	do {
		if ((HWND)w->ptr == (HWND)hwnd) return w;
		w = (Window)w->prev;
	} while (w != NULL);

	return NULL;
}

#endif
