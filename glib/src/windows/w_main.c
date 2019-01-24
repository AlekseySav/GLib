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
	if FAILED(w) return DefWinProc;
	EventArgs args;

	if (msg == WM_DESTROY)
	{
		glib_windows_count--;
		if (w->prev != NULL) ((Window)w->prev)->next = w->next;
		if (w->next != NULL) ((Window)w->next)->prev = w->prev;
		glibAddWindowFlag(SYS_CLOSED, w);
		args.msg = EVENT_CLOSED;
		glibRunWindowEvent(w, &args);

		if (glib_windows_count == 0) PostQuitMessage(0);
		return 0;
	}

	if (w == NULL) return DefWinProc;
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
	PAINTSTRUCT ps;
	HGDIOBJ oldBitmap;
	HBITMAP hBitmap;
	BitmapInfoHeader info = glibCreateBitmapInfoHeader(w->width, w->height);
	void * memalloc = malloc(info.biSize);
	byte * ptr = (byte *)memalloc;

	if (ptr == NULL) return;

	HDC hdc = BeginPaint((HWND)w->ptr, &ps);

	hBitmap = CreateDIBSection(hdc, (const BITMAPINFO *)&info, DIB_RGB_COLORS, &ptr, NULL, 0);
	if (ptr == NULL) return;

	u_int add = 0;
	u_int imptr = info.biWidth * (info.biHeight - 1) * 4;
	for (long y = 0; y < info.biHeight; y++, imptr -= 8 * info.biWidth)
		for (long x = 0; x < info.biWidth; x++, add += 4, imptr += 4)
		{
			*(ptr + add) = *(im->image + imptr + 3);
			*(ptr + add + 1) = *(im->image + imptr + 2);
			*(ptr + add + 2) = *(im->image + imptr + 1);
			*(ptr + add + 3) = *(im->image + imptr);
		}

	HDC hdcMem = CreateCompatibleDC(hdc);
	oldBitmap = SelectObject(hdcMem, hBitmap);

	BitBlt(hdc, 0, 0, info.biWidth, info.biHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
	EndPaint((HWND)w->ptr, &ps);

	free(memalloc);
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
			Window w = glib_window_draw;
			if (w == NULL) continue;
			if(!(w->flags & SYS_REDRAW)) continue;

			EventArgs args; args.msg = EVENT_DRAW;
			WIN_Paint(&args, w);
			glib_drawing = false;
		}
	}

	return (int)msg.lParam;
}

#endif
