#include "src/glib_src.h"

#ifdef _USES_WINDOWS_DRIVER

#ifdef UNICODE
LPCWSTR WIN_cname = L"GLIB_WNDCLASS";
#else
char * WIN_cname = "GLIB_WNDCLASS";
#endif
HINSTANCE WIN_hinstance;
bool WIN_wmpaint = false;

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

	exit(res);
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
		glibStartWindowDraw(w);
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
			args.flag1 = EVENT_CLOSING_CLOSE;
			args.msg = EVENT_CLOSING;
			if (glibRunWindowEvent(w, &args)) {
				if (args.flag1 == EVENT_CLOSING_CLOSE) glibCloseWindow(w);
				return 0;
			}
			return 1;
		}
		break;
	case WM_SIZE:
		w->width = LITTLE_WORD(lp);
		w->height = BIG_WORD(lp);

		if (glibCheckWindowEvent(w, EVENT_RESIZE)) {
			args.msg = EVENT_RESIZE;
			args.flag1 = lp;

			if (wp == SIZE_MINIMIZED) args.flag2 = EVENT_RESIZE_MINIMIZED;
			if (wp == SIZE_MAXIMIZED) args.flag2 = EVENT_RESIZE_MAXIMIZED;
			else args.flag2 = EVENT_RESIZE_RESTORED;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MOVE:
		w->x = LITTLE_WORD(lp);
		w->y = BIG_WORD(lp);

		if (glibCheckWindowEvent(w, EVENT_MOVED)) {
			args.msg = EVENT_MOVED;
			args.flag1 = lp;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_PAINT:
		WIN_wmpaint = true;
		WIN_Paint(&args, w);
		WIN_wmpaint = false;
			if (w->flags & SYS_REDRAW)
				return DefWinProc;
			return 0;
		break;
	case WM_LBUTTONDOWN:
		if (glibCheckWindowEvent(w, EVENT_MOUSEDOWN)) {
			args.msg = EVENT_MOUSEDOWN;
			args.flag1 = lp;
			args.flag2 = MOUSE_LEFTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MBUTTONDOWN:
		if (glibCheckWindowEvent(w, EVENT_MOUSEDOWN)) {
			args.msg = EVENT_MOUSEDOWN;
			args.flag1 = lp;
			args.flag2 = MOUSE_MIDDLEBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_RBUTTONDOWN:
		if (glibCheckWindowEvent(w, EVENT_MOUSEDOWN)) {
			args.msg = EVENT_MOUSEDOWN;
			args.flag1 = lp;
			args.flag2 = MOUSE_RIGHTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_XBUTTONDOWN:
		if (glibCheckWindowEvent(w, EVENT_MOUSEDOWN)) {
			args.msg = EVENT_MOUSEDOWN;
			args.flag1 = lp;
			if ((u_int16)((wp >> 16) & 0xffff) == 1) args.flag2 = MOUSE_X1BUTTON;
			else args.flag2 = MOUSE_X2BUTTON;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_LBUTTONUP:
		if (glibCheckWindowEvent(w, EVENT_MOUSEUP)) {
			args.msg = EVENT_MOUSEUP;
			args.flag1 = lp;
			args.flag2 = MOUSE_LEFTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MBUTTONUP:
		if (glibCheckWindowEvent(w, EVENT_MOUSEUP)) {
			args.msg = EVENT_MOUSEUP;
			args.flag1 = lp;
			args.flag2 = MOUSE_MIDDLEBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_RBUTTONUP:
		if (glibCheckWindowEvent(w, EVENT_MOUSEUP)) {
			args.msg = EVENT_MOUSEUP;
			args.flag1 = lp;
			args.flag2 = MOUSE_RIGHTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_XBUTTONUP:
		if (glibCheckWindowEvent(w, EVENT_MOUSEUP)) {
			args.msg = EVENT_MOUSEUP;
			args.flag1 = lp;
			if ((u_int16)((wp >> 16) & 0xffff) == 1) args.flag2 = MOUSE_X1BUTTON;
			else args.flag2 = MOUSE_X2BUTTON;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_LBUTTONDBLCLK:
		if (glibCheckWindowEvent(w, EVENT_DOUBLECLICK)) {
			args.msg = EVENT_DOUBLECLICK;
			args.flag1 = lp;
			args.flag2 = MOUSE_LEFTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MBUTTONDBLCLK:
		if (glibCheckWindowEvent(w, EVENT_DOUBLECLICK)) {
			args.msg = EVENT_DOUBLECLICK;
			args.flag1 = lp;
			args.flag2 = MOUSE_MIDDLEBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_RBUTTONDBLCLK:
		if (glibCheckWindowEvent(w, EVENT_DOUBLECLICK)) {
			args.msg = EVENT_DOUBLECLICK;
			args.flag1 = lp;
			args.flag2 = MOUSE_RIGHTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_XBUTTONDBLCLK:
		if (glibCheckWindowEvent(w, EVENT_DOUBLECLICK)) {
			args.msg = EVENT_DOUBLECLICK;
			args.flag1 = lp;
			if(BIG_WORD(wp) == 1) args.flag2 = MOUSE_X1BUTTON;
			else args.flag2 = MOUSE_X2BUTTON;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MOUSEMOVE:
		Mouse.x = ((int32)(int16)LITTLE_WORD(lp));
		Mouse.y = ((int32)(int16)BIG_WORD(lp));
		if (glibCheckWindowEvent(w, EVENT_MOUSEMOVE)) {
			args.msg = EVENT_MOUSEMOVE;
			args.flag1 = lp;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_KEYDOWN:
		if (glibCheckWindowEvent(w, EVENT_KEYDOWN)) {
			args.msg = EVENT_KEYDOWN;
			args.flag1 = wp;	
			return !glibRunWindowEvent(w, &args);
		}
		break;
	}
	return DefWinProc;
}

void WIN_DrawWindow(Image im, Window w, Point min, Point max)
{
	byte * ptr = (byte *)malloc(im->width * im->height * 4);
	if (ptr == NULL) return;

	HDC hdc;
	PAINTSTRUCT p;
	if (WIN_wmpaint) hdc = BeginPaint((HWND)w->ptr, &p);
	else hdc = GetDC((HWND)w->ptr);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, max.x - min.x, max.y - min.y);
	if (ptr == NULL) return;

	u_int add = 0;
	for (long y = min.y; y < (long)max.y; y++)
		for (long x = min.x; x < (long)max.x; x++, add += 4)
		{
			*(ptr + add) = *(im->image + add + 3);
			*(ptr + add + 1) = *(im->image + add + 2);
			*(ptr + add + 2) = *(im->image + add + 1);
			*(ptr + add + 3) = *(im->image + add);
		}

	SetBitmapBits(hBitmap, (max.x - min.x) * (max.y - min.y) * 4, ptr);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);

	BitBlt(hdc, 0, 0, (max.x - min.x), (max.y - min.y), hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);

	DeleteObject(oldBitmap);
	DeleteObject(hBitmap);

	DeleteDC(hdcMem);
	if (WIN_wmpaint) EndPaint((HWND)w->ptr, &p);
	ReleaseDC((HWND)w->ptr, hdc);

	free(ptr);
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

		EventArgs args;
		args.msg = EVENT_BASIC;
		Window w = glib_window_last;
		while (w != NULL) 
		{
			if(glibCheckWindowEvent(w, EVENT_BASIC)) glibRunWindowEvent(w, &args);
			w = (Window)w->prev;
		}
	}

	return (int)msg.lParam;
}

#endif
