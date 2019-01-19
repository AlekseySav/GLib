#include "glib_src.h"

bool glibCorrectWindowsFlag(u_int32 flags)
{
	if (flags & SHOW_MAXIMAZE && flags & SHOW_MINIMAZE) return false;
	if (flags & STYLE_FULLSCREEN && flags & STYLE_NORMAL) return false;
	if (flags & STYLE_NOBORDER && flags & STYLE_NORMAL) return false;
	return true;
}

EXTERN void glibAddWindowFlag(u_int32 flag, Window w)
{
	w->flags |= flag;
}

EXTERN void glibRemoveWindowFlag(u_int32 flag, Window w)
{
	u_int32 rm = - flag;
	w->flags &= rm;
}

Window glibCreateWindow(char * title, int x, int y, int width, int height, u_int32 flags, void * parent)
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

	if FAILED(EX_CreateWindow(w)) return NULL;
	return w;
}

int glibShowWindow(Window w)
{
	return EX_ShowWindow(w);
}
