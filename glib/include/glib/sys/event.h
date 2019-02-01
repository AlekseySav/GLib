#pragma once

#define EVENT_NULL		0x0000
#define EVENT_BASIC		0x0001
#define EVENT_SHOWN		0x0002
#define EVENT_CLOSING	0x0004
#define EVENT_CLOSED	0x0008
#define EVENT_RESIZE	0x0010
#define EVENT_MOVED		0x0020
#define EVENT_DRAW		0x0040

#define EVENT_ALL		0x007f

typedef struct EventArgs__ {
	Handle handle;
	u_int msg;
	u_int32 flags;
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
} EventHandles;

EXTERN void glibSetEvent(EventHandles * events, EventHandle handle, u_int type);
EXTERN bool glibRunEvent(EventHandles * events, EventArgs * args);
EXTERN bool glibCheckEvent(u_int type, EventHandles * events);
