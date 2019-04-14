#include "src/glib_src.h"

u_int glib_windows_count = 0;
Window__ * glib_window_last = NULL;

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
	w->flags &= ~flag;
	return glibCheckWindow(w);
}

Window glibCreateWindow(char * title, u_int x, u_int y, u_int width, u_int height, u_int32 flags, const void * parent)
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

	if (!glibCorrectWindowsFlag(flags))
		glibAddWindowFlag(SYS_FAILED, w);
	else if (EX_CreateWindow(w) < 0)
		glibAddWindowFlag(SYS_FAILED, w);
	else
	{
		glibAddWindowFlag(SYS_CREATED, w);
		w->prev = glib_window_last;
		w->next = NULL;

		if (glib_window_last != NULL)
			glib_window_last->next = w;

		glib_window_last = w;
		glib_windows_count++;
	}
	return w;
}

int glibShowWindow(Window w)
{
	if (!glibCheckWindow(w)) return -3;
	if (w->flags & SYS_SHOWN) return -2;
	glibAddWindowFlag(SYS_SHOWN, w);
	return EX_ShowWindow(w);
}

int glibCloseWindow(Window w)
{
	if (!glibCheckWindow(w)) return -3;
	if (!(w->flags & SYS_SHOWN) || w->flags & SYS_CLOSED) return -2;
	glibAddWindowFlag(SYS_CLOSED, w);
	glib_windows_count--;
	if (w->prev != NULL) ((Window)w->prev)->next = w->next;
	if (w->next != NULL) ((Window)w->next)->prev = w->prev;
	if (w == glib_window_last) glib_window_last = (Window)w->prev;
	int res = EX_CloseWindow(w);
	free(w);

	return res;
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
	return glibCheckEvent(type, &w->handles);
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

bool glibDrawImage(Image im, Window w, Point min, Point max, bool release)
{
	if (!(w->flags & SYS_REDRAW)) return false;
	if (!glibCheckWindow(w)) return false;
	if (im == NULL) return false;

	EX_DrawWindow(im, w, min, max);
	glibRemoveWindowFlag(SYS_REDRAW, w);

	if(release) im->Release();
	return true;
}

bool glibDrawImage(Image im, Window w, bool release)
{
	return glibDrawImage(im, w, Point { 0, 0 }, Point { im->width, im->height }, release);
}

void glibStartWindowDraw(Window w)
{
	glibAddWindowFlag(SYS_REDRAW, w);
}
