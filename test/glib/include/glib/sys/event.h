#if !defined(_GLIB_SYS_EVENT_) && defined(_GLIB)
#define _GLIB_SYS_EVENT_

#define EVENT_NULL			0x0000
#define EVENT_BASIC			0x0001
#define EVENT_SHOWN			0x0002
#define EVENT_CLOSING		0x0004		//set flag: to close handle or not (defined in event.h)
#define EVENT_CLOSED		0x0008
#define EVENT_RESIZE		0x0010		//get flag: resize type (defined in event.h)
#define EVENT_MOVED			0x0020
#define EVENT_REDRAW		0x0040
#define EVENT_DRAWING		0x0041		//EVENT_BASIC | EVENT_REDRAW
#define EVENT_MOUSEDOWN		0x0080		//get flag: mouse button (defined in std.h)
#define EVENT_CLICK			EVENT_MOUSEDOWN
#define EVENT_MOUSEUP		0x0100		//get flag: mouse button (defined in std.h)
#define EVENT_DOUBLECLICK	0x0200		//get flag: mouse button (defined in std.h)
#define EVENT_MOUSEMOVE		0x0400
#define EVENT_KEYDOWN		0x0800		//get flag: key code (defined in std.h)

#define EVENT_ALL			0x0fff

typedef struct EventArgs__ {
	Handle handle;
	u_int msg;
	u_int32 flag;
} EventArgs;

typedef void(*EventHandle)(EventArgs * args);

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

#endif
