#pragma once

#define EVENT_NULL			0x0000
#define EVENT_BASIC			0x0001
#define EVENT_SHOWN			0x0002
#define EVENT_CLOSING		0x0004		//set flag1: to close window or not (defined in event.h)
#define EVENT_CLOSED		0x0008
#define EVENT_RESIZE		0x0010		//get flag1: handle size, get flag2: resize type (defined in event.h)
#define EVENT_MOVED			0x0020		//get flag1: handle coordinates
#define EVENT_DRAW			0x0040
#define EVENT_MOUSEDOWN		0x0080		//get flag1: mouse coordinates, get flag2: mouse button (defined in std.h)
#define EVENT_CLICK			EVENT_MOUSEDOWN
#define EVENT_MOUSEUP		0x0100		//get flag1: mouse coordinates, get flag2: mouse button (defined in std.h)
#define EVENT_DOUBLECLICK	0x0200		//get flag1: mouse coordinates, get flag2: mouse button (defined in std.h)
#define EVENT_MOUSEMOVE		0x0400		//get flag1: mouse coordinates
#define EVENT_KEYDOWN		0x0800		//get flag1: key code (defined in std.h)

#define EVENT_ALL			0x0fff

typedef struct EventArgs__ {
	Handle handle;
	u_int msg;
	u_int32 flag1;
	u_int32 flag2;
} EventArgs;

typedef void (* EventHandle)(EventArgs * args);

#define EVENT_CLOSING_CLOSE 0
#define EVENT_CLOSING_NOCLOSE 1

#define EVENT_RESIZE_RESTORED 0
#define EVENT_RESIZE_MINIMIZED 1
#define EVENT_RESIZE_MAXIMIZED 2

typedef struct EventHandles__ {
	EventHandle basic;
	EventHandle shown;
	EventHandle closing;
	EventHandle closed;
	EventHandle resize;
	EventHandle moved;
	EventHandle draw;
	EventHandle mousedown;
	EventHandle mouseup;
	EventHandle doubleclick;
	EventHandle mousemove;
	EventHandle keydown;
} EventHandles;

EXTERN void glibSetEvent(EventHandles * events, EventHandle handle, u_int type);
EXTERN bool glibRunEvent(EventHandles * events, EventArgs * args);
EXTERN bool glibCheckEvent(u_int type, EventHandles * events);
