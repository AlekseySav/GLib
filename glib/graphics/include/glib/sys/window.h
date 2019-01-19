#pragma once

#define EX_CreateWindow _FUNCTION_HANDLE(CreateWindow)
#define EX_ShowWindow _FUNCTION_HANDLE(ShowWindow)
#define EX_ChangeWindowPos _FUNCTION_HANDLE(ChangeWindowPos)
#define EX_ChangeWindowTitle _FUNCTION_HANDLE(ChangeWindowTitle)
#define EX_ChangeWindowFlags _FUNCTION_HANDLE(ChangeWindowFlags)

#define MAX_TITLE_LENGTH 100

typedef enum WINDOW_EVENTS
{
	EVENT_WINDOW_SHOWN =		0x0001,
	EVENT_WINDOW_CLOSING =		0x0002,
	EVENT_WINDOW_CLOSED =		0x0004
} WINDOW_EVENTS;

typedef struct Events__ {
	u_int32 flag;

	int WindowShown;
	int WindowClosing;
	int WindowClosed;
} * Events;

typedef struct Window__ {

	const void * ptr;
	const void * parent;

	char * title;
	int x, y;
	int width, height;

	u_int32 flags;

	Events events;
} * Window;

EXTERN Window glibCreateWindow(char * title, int x, int y, int width, int height, u_int32 flags, const void * parent);
EXTERN int glibShowWindow(Window w);

#define STYLE_BASIC				0x0001
#define STYLE_NOBORDER			0x0002
#define STYLE_FULLSCREEN		0x0004
#define STYLE_MINIMIZEABLE		0x0008
#define STYLE_MAXIMAZE			0x0010
#define STYLE_THICKFRAME		0x0020
#define STYLE_OVERLAPPED		0x0040

#define STYLE_RESIZABLE			0x0030	//(STYLE_THICKFRAME | STYLE_MAXIMAZE)
#define STYLE_NORMAL			0x0079	//(STYLE_BASIC | STYLE_OVERLAPPED | STYLE_MINIMIZEABLE | STYLE_RESIZABLE)

#define STATE_NORMAL			0x0000
#define STATE_MINIMIZE			0x0080
#define STATE_MAXIMIZE			0x0100
#define STATE_DISABLED			0x0200

#define SHOW_NOACTIVE			0x0400

#define SYS_CREATED				0x0800
#define SYS_SHOWN				0x1000

EXTERN void glibAddWindowFlag(u_int32 flag, Window w);
EXTERN void glibRemoveWindowFlag(u_int32 flag, Window w);
EXTERN bool glibCorrectWindowsFlag(u_int32 flags);

#define RELOAD_POSITION			0x0001
#define RELOAD_TITLE			0x0002
#define RELOAD_FLAGS			0x0004
#define RELOAD_ALL				0x0007	//(RELOAD_TITLE | RELOAD_FLAGS | RELOAD_POSITION)

EXTERN void glibReloadWindow(Window w, int reload);

typedef void Event_WindowShown(Window w);
typedef void Event_WindowClosing(Window w);
typedef void Event_WindowClosed(Window w);

EXTERN void glibLogEvent(Window w, WINDOW_EVENTS event, int handler);
