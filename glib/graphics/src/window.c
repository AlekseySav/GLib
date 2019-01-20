#include "glib_src.h"

u_int windows_count = 0;
Window__ * last = NULL;

bool glibCheckWindow(Window w)
{
	if (!(w->flags & SYS_CREATED)) return false;
	if (w->flags & SYS_FAILED) return false;
	if (w->flags & SYS_CLOSED) return false;
	return glibCorrectWindowsFlag(w->flags);
}

bool glibCorrectWindowsFlag(u_int32 flags)
{
	if (flags & STATE_MAXIMIZE && flags & STATE_MINIMIZE) return false;
	if (flags & STYLE_FULLSCREEN && flags & STYLE_NORMAL) return false;
	if (flags & STYLE_NOBORDER && flags & STYLE_NORMAL) return false;
	return true;
}

bool glibAddWindowFlag(u_int32 flag, Window w)
{
	w->flags |= flag;
	return glibCheckWindow(w);
}

bool glibRemoveWindowFlag(u_int32 flag, Window w)
{
	u_int32 rm = (-1) ^ flag;
	w->flags &= rm;
	return glibCheckWindow(w);
}

Window glibCreateWindow(char * title, int x, int y, int width, int height, u_int32 flags, const void * parent)
{
	Window w = (Window)malloc(sizeof(Window__));
	w->title = title;
	w->x = x;
	w->y = y;
	w->width = width;
	w->height = height;
	w->flags = flags;
	w->parent = parent;
	glibSetWindowEvent(w, NULL, EVENT_ALL);

	if(!glibCorrectWindowsFlag(flags))
		glibAddWindowFlag(SYS_FAILED, w);
	else if FAILED(EX_CreateWindow(w))
		glibAddWindowFlag(SYS_FAILED, w);
	else
	{
		glibAddWindowFlag(SYS_CREATED, w);
		w->prev = last;
		w->next = NULL;

		if (last != NULL)
			last->next = w;

		last = w;
		windows_count++;
	}
	return w;
}

int glibShowWindow(Window w)
{
	if(!glibCheckWindow(w)) return -3;
	if (w->flags & SYS_SHOWN) return -2;
	glibAddWindowFlag(SYS_SHOWN, w);
	return EX_ShowWindow(w);
}

int glibCloseWindow(Window w)
{
	if (!glibCheckWindow(w)) return -3;
	if (!(w->flags & SYS_SHOWN) || w->flags & SYS_CLOSED) return -2;
	glibAddWindowFlag(SYS_CLOSED, w);
	return EX_CloseWindow(w);
}

int glibReloadWindow(Window w, int reload)
{
	if (!glibCheckWindow(w)) return -1;
	if (reload & RELOAD_TITLE) EX_ChangeWindowTitle(w);
	if (reload & RELOAD_POSITION) EX_ChangeWindowPos(w);
	if (reload & RELOAD_FLAGS) EX_ChangeWindowFlags(w);
	return 0;
}

bool glibCheckWindowEvent(Window w, u_int type)
{
	if (!glibCheckWindow(w)) return false;

	switch (type)
	{
	case EVENT_SHOWN:
		return w->handles.shown != NULL;
	case EVENT_CLOSING:
		return w->handles.closing != NULL;
	case EVENT_CLOSED:
		return w->handles.closed != NULL;
	case EVENT_RESIZE:
		return w->handles.resize != NULL;
	case EVENT_MOVED:
		return w->handles.moved != NULL;
	default:
		return false;
	}
}

void glibSetWindowEvent(Window w, EventHandle handle, u_int etypes)
{
	glibSetEvent(&w->handles, handle, etypes);
}

bool glibRunWindowEvent(Window w, EventArgs * args)
{
	args->handle = w;
	return glibRunEvent(&w->handles, args);
}
