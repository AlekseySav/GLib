#include "src/glib_src.h"

#ifdef _USES_WINDOWS_DRIVER

#ifdef UNICODE
LPCWSTR WIN_cname = L"GLIB_WNDCLASS";
#else
char * WIN_cname = "GLIB_WNDCLASS";
#endif
HINSTANCE WIN_hinstance;

int WIN_Ascii_to_UNICODE(const char * ibuf, LPWSTR obuf)
{
	return MultiByteToWideChar(CP_OEMCP, 0, ibuf, -1, obuf, strlen(ibuf) + 1);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprev, LPSTR str, int cmd)
{
	WIN_hinstance = hInst;

	WIN_Initialize();

	int argc = 0;
	u_int max_cnt = 0;
	while (*(str + max_cnt) || *(str + max_cnt + 1))
	{
		max_cnt++;
		if(*(str + max_cnt) == 0)
		argc++;
	}

	u_int * argv;
	if (argc != 0) {
		argv = (u_int *)malloc((u_int)argc * (u_int)sizeof(u_int));

		u_int ptr = 0;
		u_int last = 0;
		for (u_int i = 0; i <= max_cnt; i++) {
			if (str[i] == 0)
			{
				*(argv + ptr) = (int)&str[last];
				last = i + 1;
				ptr++;
			}
		}
	}
	else argv = NULL;

	int res = gmain((char **)argv, argc);

	return res;
}

int WIN_Initialize()
{
	return WIN_RegisterClass();
}

int WIN_RegisterClass()
{
	WNDCLASS c;
	c.cbClsExtra = 0;
	c.cbWndExtra = 0;
	c.hbrBackground = (HBRUSH)COLOR_WINDOW;
	c.hCursor = LoadCursor(NULL, IDC_ARROW);
	c.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	c.hInstance = WIN_hinstance;
	c.lpfnWndProc = WndProc;
	c.lpszClassName = WIN_cname;
	c.lpszMenuName = NULL;
	c.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&c)) return -1;

	return 0;
}

int WIN_Paint(EventArgs * args, Window w)
{
	if (glibCheckWindowEvent(w, EVENT_DRAW)) {
		glib_drawing = true;
		args->msg = EVENT_DRAW;

		return glibRunWindowEvent(w, args);
	}
	return 0;
}

#define DefWinProc DefWindowProc(wnd, msg, wp, lp)
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Window w = EX_GetWindow(wnd);
	if (w == NULL) return DefWinProc;
	if FAILED(w) return DefWinProc;
	EventArgs args;

	if (msg == WM_DESTROY)
	{
		args.msg = EVENT_CLOSED;
		glibRunWindowEvent(w, &args);
		glibCloseWindow(w);

		if (glib_windows_count == 0) PostQuitMessage(0);
		return 0;
	}

	switch (msg)
	{
	case WM_SHOWWINDOW:
		if (glibCheckWindowEvent(w, EVENT_SHOWN)) {
			args.msg = EVENT_SHOWN;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_CLOSE:
		if (glibCheckWindowEvent(w, EVENT_CLOSING)) {
			args.flags = EVENT_CLOSING_CLOSE;
			args.msg = EVENT_CLOSING;
			if (glibRunWindowEvent(w, &args)) {
				if (args.flags == EVENT_CLOSING_CLOSE) glibCloseWindow(w);
				return 0;
			}
			return 1;
		}
		break;
	case WM_SIZE:
		w->width = LOWORD(lp);
		w->height = HIWORD(lp);

		if (glibCheckWindowEvent(w, EVENT_RESIZE)) {
			args.msg = EVENT_RESIZE;
			if (wp == SIZE_MINIMIZED) args.flags = EVENT_RESIZE_MINIMIZED;
			if (wp == SIZE_MAXIMIZED) args.flags = EVENT_RESIZE_MAXIMIZED;
			else args.flags = EVENT_RESIZE_RESTORED;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MOVE:
		w->x = (int)(short)LOWORD(lp);
		w->y = (int)(short)HIWORD(lp);

		if (glibCheckWindowEvent(w, EVENT_MOVED)) {
			args.msg = EVENT_MOVED;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_PAINT:
		WIN_Paint(&args, w);
			if (glib_drawing)
				return DefWinProc;
			return 0;
		break;
	}
	return DefWinProc;
}

void WIN_DrawWindow(Image im, Window w)
{
	byte * ptr = (byte *)malloc(im->width * im->height * 4);
	if (ptr == NULL) return;

	HDC hdc = GetDC((HWND)w->ptr);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, im->width, im->height);
	if (ptr == NULL) return;

	u_int add = 0;
	for (long y = 0; y < (long)im->height; y++)
		for (long x = 0; x < (long)im->width; x++, add += 4)
		{
			*(ptr + add) = *(im->image + add + 3);
			*(ptr + add + 1) = *(im->image + add + 2);
			*(ptr + add + 2) = *(im->image + add + 1);
			*(ptr + add + 3) = *(im->image + add);
		}

	SetBitmapBits(hBitmap, im->width * im->height * 4, ptr);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);

	BitBlt(hdc, 0, 0, im->width, im->height, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);

	DeleteObject(oldBitmap);
	DeleteObject(hBitmap);

	DeleteDC(hdcMem);
	ReleaseDC((HWND)w->ptr, hdc);

	free(ptr);
	glibFreeImage(im);
}

int WIN_MainLoop()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, -1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			EventArgs args;
			Window w = glib_window_last;
			while (w != NULL) {
				if (w->flags & SYS_REDRAW)
					WIN_Paint(&args, w);
				else
				{
					args.msg = EVENT_BASIC;
					glibRunWindowEvent(w, &args);
				}

				w = (Window)w->prev;
			}

			glib_drawing = false;
		}
	}

	return (int)msg.lParam;
}

#endif
