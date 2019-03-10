#include "src/glib_src.h"

#if _USER_DRIVER == _WINDOWS_DRIVER

#ifdef UNICODE
LPCWSTR WIN_cname = L"GLIB_WNDCLASS";
#else
char * WIN_cname = "GLIB_WNDCLASS";
#endif
HINSTANCE WIN_hinstance;
bool WIN_wmpaint = false;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprev, LPSTR str, int cmd)
{
	WIN_hinstance = hInst;

	WIN_Initialize();

	int argc = 0;
	u_int max_cnt = 0;
	while (*(str + max_cnt) || *(str + max_cnt + 1))
	{
		max_cnt++;
		if (*(str + max_cnt) == 0)
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
				*(argv + ptr) = (u_int)(int)(void *)&str[last];
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
	glibSetDefaultLanguage();
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
	if WINDOW_FAILED(w) return DefWinProc;
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
			args.flag = EVENT_CLOSING_CLOSE;
			args.msg = EVENT_CLOSING;
			if (glibRunWindowEvent(w, &args)) {
				if (args.flag == EVENT_CLOSING_CLOSE) glibCloseWindow(w);
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

			if (wp == SIZE_MINIMIZED) args.flag = EVENT_RESIZE_MINIMIZED;
			if (wp == SIZE_MAXIMIZED) args.flag = EVENT_RESIZE_MAXIMIZED;
			else args.flag = EVENT_RESIZE_RESTORED;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MOVE:
		w->x = LITTLE_WORD(lp);
		w->y = BIG_WORD(lp);

		if (glibCheckWindowEvent(w, EVENT_MOVED)) {
			args.msg = EVENT_MOVED;
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
			args.flag = MOUSE_LEFTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MBUTTONDOWN:
		if (glibCheckWindowEvent(w, EVENT_MOUSEDOWN)) {
			args.msg = EVENT_MOUSEDOWN;
			args.flag = MOUSE_MIDDLEBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_RBUTTONDOWN:
		if (glibCheckWindowEvent(w, EVENT_MOUSEDOWN)) {
			args.msg = EVENT_MOUSEDOWN;
			args.flag = MOUSE_RIGHTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_XBUTTONDOWN:
		if (glibCheckWindowEvent(w, EVENT_MOUSEDOWN)) {
			args.msg = EVENT_MOUSEDOWN;
			if ((u_int16)((wp >> 16) & 0xffff) == 1) args.flag = MOUSE_X1BUTTON;
			else args.flag = MOUSE_X2BUTTON;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_LBUTTONUP:
		if (glibCheckWindowEvent(w, EVENT_MOUSEUP)) {
			args.msg = EVENT_MOUSEUP;
			args.flag = MOUSE_LEFTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MBUTTONUP:
		if (glibCheckWindowEvent(w, EVENT_MOUSEUP)) {
			args.msg = EVENT_MOUSEUP;
			args.flag = MOUSE_MIDDLEBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_RBUTTONUP:
		if (glibCheckWindowEvent(w, EVENT_MOUSEUP)) {
			args.msg = EVENT_MOUSEUP;
			args.flag = MOUSE_RIGHTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_XBUTTONUP:
		if (glibCheckWindowEvent(w, EVENT_MOUSEUP)) {
			args.msg = EVENT_MOUSEUP;
			if ((u_int16)((wp >> 16) & 0xffff) == 1) args.flag = MOUSE_X1BUTTON;
			else args.flag = MOUSE_X2BUTTON;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_LBUTTONDBLCLK:
		if (glibCheckWindowEvent(w, EVENT_DOUBLECLICK)) {
			args.msg = EVENT_DOUBLECLICK;
			args.flag = MOUSE_LEFTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MBUTTONDBLCLK:
		if (glibCheckWindowEvent(w, EVENT_DOUBLECLICK)) {
			args.msg = EVENT_DOUBLECLICK;
			args.flag = MOUSE_MIDDLEBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_RBUTTONDBLCLK:
		if (glibCheckWindowEvent(w, EVENT_DOUBLECLICK)) {
			args.msg = EVENT_DOUBLECLICK;
			args.flag = MOUSE_RIGHTBUTTON;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_XBUTTONDBLCLK:
		if (glibCheckWindowEvent(w, EVENT_DOUBLECLICK)) {
			args.msg = EVENT_DOUBLECLICK;
			if (BIG_WORD(wp) == 1) args.flag = MOUSE_X1BUTTON;
			else args.flag = MOUSE_X2BUTTON;

			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_MOUSEMOVE:
		Mouse.x = ((int32)(int16)LITTLE_WORD(lp));
		Mouse.y = ((int32)(int16)BIG_WORD(lp));
		if (glibCheckWindowEvent(w, EVENT_MOUSEMOVE)) {
			args.msg = EVENT_MOUSEMOVE;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	case WM_KEYDOWN:
		if (glibCheckWindowEvent(w, EVENT_KEYDOWN)) {
			args.msg = EVENT_KEYDOWN;
			args.flag = (u_int32)wp;
			return !glibRunWindowEvent(w, &args);
		}
		break;
	}
	return DefWinProc;
}

void WIN_DrawWindow(Image im, Window w, Point min, Point max)
{
	byte * ptr = (byte *)malloc((max.x - min.x) * (max.y - min.y) * 4);
	if (ptr == NULL) return;

	HDC hdc;
	PAINTSTRUCT p;
	if (WIN_wmpaint) hdc = BeginPaint((HWND)w->ptr, &p);
	else hdc = GetDC((HWND)w->ptr);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, max.x - min.x, max.y - min.y);

	u_int add = 0, iadd = (min.y * im->width + min.x) * 4;
	u_int addad = (min.x - max.x + im->width) * 4;
	for (long y = min.y; y < (long)max.y; y++)
	{
		for (long x = min.x; x < (long)max.x; x++, add += 4, iadd += 4)
		{
			*(ptr + add) = *(im->image + iadd + 3);
			*(ptr + add + 1) = *(im->image + iadd + 2);
			*(ptr + add + 2) = *(im->image + iadd + 1);
			*(ptr + add + 3) = *(im->image + iadd);
		}
		iadd += addad;
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
		else
		{
			EventArgs args;
			args.msg = EVENT_BASIC;
			Window w = glib_window_last;
			while (w != NULL)
			{
				if (glibCheckWindowEvent(w, EVENT_BASIC)) glibRunWindowEvent(w, &args);
				w = (Window)w->prev;
			}
		}
	}

	return (int)msg.lParam;
}

int WIN_CreateConsole() 
{/*
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
*/
	return 0;
}

HBITMAP WIN_HBITMAP(HDC dc, Point pos, LPWSTR text, Image im, ARGB font, ARGB back)
{
	byte * ptr = (byte *)malloc(im->width * im->height * 4);

	HDC compatibleDeviceContext = CreateCompatibleDC(dc);
	HBITMAP bitmapHandle = CreateCompatibleBitmap(dc, im->width, im->height);

	u_int add = 0, iadd = 0;
	for (long y = 0; y < (long)im->height; y++)
		for (long x = 0; x < (long)im->width; x++, add += 4, iadd += 4)
		{
			*(ptr + add) = *(im->image + iadd + 3);
			*(ptr + add + 1) = *(im->image + iadd + 2);
			*(ptr + add + 2) = *(im->image + iadd + 1);
			*(ptr + add + 3) = *(im->image + iadd);
		}

	SetBitmapBits(bitmapHandle, im->width * im->height * 4, ptr);
	free(ptr);

	HGDIOBJ previousSelectedHandle = SelectObject(compatibleDeviceContext, bitmapHandle);

	SetTextColor(compatibleDeviceContext, RGB(font.r, font.g, font.b));
	if (back.a != 0)
		SetBkColor(compatibleDeviceContext, RGB(back.r, back.g, back.b));
	else
		SetBkMode(compatibleDeviceContext, TRANSPARENT);
	TextOut(compatibleDeviceContext, pos.x, pos.y, text, strlen(text));
	
	SelectObject(compatibleDeviceContext, previousSelectedHandle);
	DeleteDC(compatibleDeviceContext);
	return bitmapHandle;
}

void WIN_DrawText(Image im, Point pos, char * str, u_int32 format, ARGB font, ARGB back)
{
	HDC deviceContext = GetDC(nullptr);
	
	w_char * wstr = (w_char *)malloc((strlen(str) + 1) * sizeof(w_char));
	glibConvertString(str, wstr);

	HBITMAP capturedBitmap = WIN_HBITMAP(deviceContext, pos, wstr, im, font, back);
	free(wstr);

	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);

	GetDIBits(deviceContext, capturedBitmap, 0, 0, nullptr, &bitmapInfo, DIB_RGB_COLORS);
	byte * pixels = new byte[bitmapInfo.bmiHeader.biSizeImage];

	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	GetDIBits(deviceContext, capturedBitmap, 0, bitmapInfo.bmiHeader.biHeight, (LPVOID)pixels, &bitmapInfo, DIB_RGB_COLORS);

	int iy = im->width * im->height * 4 - im->width * 4;
	for(int y = 0; y < im->height; y++, iy -= im->width * 4)
		for (int x = 0; x < im->width; x++)
		{
			byte * cls = (pixels + y * im->width * 4 + x * 4);
			ARGB clr = ARGB{ cls[3], cls[2], cls[1], cls[0] };
			*(ARGB *)(void *)(im->image + iy + x * 4) = clr;
		}

	DeleteObject(capturedBitmap);
	ReleaseDC(nullptr, deviceContext);
	delete[] pixels;
}

#endif
