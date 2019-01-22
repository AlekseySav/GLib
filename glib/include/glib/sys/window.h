#pragma once

#define EX_CreateWindow _FUNCTION_HANDLE(CreateWindow)
#define EX_ShowWindow _FUNCTION_HANDLE(ShowWindow)
#define EX_CloseWindow _FUNCTION_HANDLE(CloseWindow)
#define EX_ChangeWindowPos _FUNCTION_HANDLE(ChangeWindowPos)
#define EX_ChangeWindowTitle _FUNCTION_HANDLE(ChangeWindowTitle)
#define EX_ChangeWindowFlags _FUNCTION_HANDLE(ChangeWindowFlags)
#define EX_GetWindow _FUNCTION_HANDLE(GetWindow)

#ifndef MAX_TITLE_LENGTH
	#define MAX_TITLE_LENGTH 100
#endif

typedef struct Window__ {

	const void * ptr;
	const void * parent;

	char * title;
	u_int x, y;
	u_int width, height;

	u_int32 flags;

	const void * prev, *next;

	EventHandles handles;
} * Window;

EXTERN bool glibCheckWindow(Window w);

EXTERN Window glibCreateWindow(char * title, u_int x, u_int y, u_int width, u_int height, u_int32 flags, const void * parent);
EXTERN int glibShowWindow(Window w);
EXTERN int glibCloseWindow(Window w);

#define WINDOW_DEFAULT_POSITION	0x80000000

#define STYLE_BASIC				0x00000001
#define STYLE_NOBORDER			0x00000002
#define STYLE_FULLSCREEN		0x00000004
#define STYLE_MINIMIZEABLE		0x00000008
#define STYLE_MAXIMAZE			0x00000010
#define STYLE_THICKFRAME		0x00000020
#define STYLE_OVERLAPPED		0x00000040

#define STYLE_RESIZABLE			0x00000030	//(STYLE_THICKFRAME | STYLE_MAXIMAZE)
#define STYLE_NORMAL			0x00000079	//(STYLE_BASIC | STYLE_OVERLAPPED | STYLE_MINIMIZEABLE | STYLE_RESIZABLE)

#define STATE_NORMAL			0x00000000
#define STATE_MINIMIZE			0x00000080
#define STATE_MAXIMIZE			0x00000100
#define STATE_DISABLED			0x00000200

#define SHOW_NOACTIVE			0x00000400

#define SYS_FAILED				0x00000800
#define SYS_CREATED				0x00001000
#define SYS_SHOWN				0x00002000
#define SYS_CLOSED				0x00004000

#define WINDOW_FAILED(w) (w->flags & SYS_FAILED)

EXTERN bool glibAddWindowFlag(u_int32 flag, Window w);
EXTERN bool glibRemoveWindowFlag(u_int32 flag, Window w);
EXTERN bool glibCorrectWindowsFlag(u_int32 flags);

#define RELOAD_POSITION			0x00000001
#define RELOAD_TITLE			0x00000002
#define RELOAD_FLAGS			0x00000004
#define RELOAD_ALL				0x00000007	//(RELOAD_TITLE | RELOAD_FLAGS | RELOAD_POSITION)

EXTERN int glibReloadWindow(Window w, int reload);

EXTERN bool glibCheckWindowEvent(Window w, u_int type);
EXTERN void glibSetWindowEvent(Window w, EventHandle handle, u_int etypes);
EXTERN bool glibRunWindowEvent(Window w, EventArgs * args);