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

#define DefWinProc DefWindowProc(wnd, msg, wp, lp)
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Window w = EX_GetWindow(wnd);
	EventArgs args;

	if (msg == WM_DESTROY)
	{
		windows_count--;
		if (w->prev != NULL) ((Window)w->prev)->next = w->next;
		if (w->next != NULL) ((Window)w->next)->prev = w->prev;
		glibAddWindowFlag(SYS_CLOSED, w);
		if (glibCheckWindowEvent(w, EVENT_CLOSED)) {
			args.msg = EVENT_CLOSED;
			glibRunWindowEvent(w, &args);
		}

		if (windows_count == 0) PostQuitMessage(0);
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
	}
	return DefWinProc;
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
	}

	return (int)msg.lParam;
}

#endif
