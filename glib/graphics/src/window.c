#include "glib_src.h"
u_int windows_count = 0;

bool glibCorrectWindowsFlag(u_int32 flags)
{
	if (flags & STATE_MAXIMIZE && flags & STATE_MINIMIZE) return false;
	if (flags & STYLE_FULLSCREEN && flags & STYLE_NORMAL) return false;
	if (flags & STYLE_NOBORDER && flags & STYLE_NORMAL) return false;
	return true;
}

void glibAddWindowFlag(u_int32 flag, Window w)
{
	w->flags |= flag;
}

void glibRemoveWindowFlag(u_int32 flag, Window w)
{
	u_int32 rm = (-1) ^ flag;
	w->flags &= rm;
}

Window glibCreateWindow(char * title, int x, int y, int width, int height, u_int32 flags, const void * parent)
{
	if (!glibCorrectWindowsFlag(flags)) return NULL;
	Window w = (Window)malloc(sizeof(struct Window__));
	w->title = title;
	w->x = x;
	w->y = y;
	w->width = width;
	w->height = height;
	w->flags = flags;
	w->parent = parent;
	w->events = malloc(sizeof(struct Events__));
	w->events->flag = 0;

	if FAILED(EX_CreateWindow(w)) return NULL;
	glibAddWindowFlag(SYS_CREATED, w);
	windows_count++;
	return w;
}

int glibShowWindow(Window w)
{
	if (w->flags & SYS_SHOWN) return -2;
	glibAddWindowFlag(SYS_SHOWN, w);
	return EX_ShowWindow(w);
}

void glibReloadWindow(Window w, int reload)
{
	if (reload & RELOAD_TITLE) EX_ChangeWindowTitle(w);
	if (reload & RELOAD_POSITION) EX_ChangeWindowPos(w);
	if (reload & RELOAD_FLAGS) EX_ChangeWindowFlags(w);
}

void glibLogEvent(Window w, WINDOW_EVENTS event, int handler)
{
	switch (event)
	{
	case EVENT_WINDOW_SHOWN: w->events->WindowShown = handler;
	default: return;
	}
}
