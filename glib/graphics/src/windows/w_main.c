#include "src/glib_src.h"

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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprev, LPSTR argv, int cmd)
{
	WIN_hinstance = hInst;

	WIN_Initialize();

	int argc = 0;
	while (*(argv + argc) != 0 || *(argv + argc + 1)) argc++;

	int res = gmain(argv, argc);

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

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(wnd, msg, w, l);
	}
	return 0;
}
