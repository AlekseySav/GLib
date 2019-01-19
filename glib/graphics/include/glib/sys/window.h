#pragma once

#ifdef _USES_WINDOWS_DRIVER
	#define _FUNCTION_HANDLE(__name__) WIN_##__name__
#elif defined(_USES_LINUX_DRIVER)
#define _FUNCTION_HANDLE(__name__) LINUX_##__name__
#elif defined(_USES_DEFAULT_DRIVER)
#define _FUNCTION_HANDLE(__name__) DEF_##__name__
#endif

#define EX_CreateWindow _FUNCTION_HANDLE(CreateWindow)
#define EX_ShowWindow _FUNCTION_HANDLE(ShowWindow)

#define MAX_TITLE_LENGTH 100

typedef struct Window__ {

	const void * ptr;
	const void * parent;

	char * title;
	int x, y;
	int width, height;

	u_int32 flags;

} * Window;

EXTERN Window glibCreateWindow(char * title, int x, int y, int width, int height, u_int32 flags, void * parent);
EXTERN int glibShowWindow(Window w);

#define STYLE_BASIC				0x001
#define STYLE_NOBORDER			0x002
#define STYLE_FULLSCREEN		0x004
#define STYLE_MINIMIZEABLE		0x008
#define STYLE_MAXIMAZE			0x010
#define STYLE_THICKFRAME		0x020
#define STYLE_OVERLAPPED		0x040

#define STYLE_RESIZABLE			0x030	//(STYLE_THICKFRAME | STYLE_MAXIMAZE)
#define STYLE_NORMAL			0x079	//(STYLE_BASIC | STYLE_OVERLAPPED | STYLE_MINIMIZEABLE | STYLE_RESIZABLE)

#define SHOW_NORMAL				0x000
#define SHOW_MINIMAZE			0x080
#define SHOW_MAXIMAZE			0x100
#define SHOW_DISABLED			0x200
#define SHOW_NOACTIVE			0x400

EXTERN void glibAddWindowFlag(u_int32 flag, Window w);
EXTERN void glibRemoveWindowFlag(u_int32 flag, Window w);
EXTERN bool glibCorrectWindowsFlag(u_int32 flags);

