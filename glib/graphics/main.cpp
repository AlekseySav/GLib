#include <glib/glib.h>

void shown(EventArgs * args)
{
	Window w = (Window)args->handle;
}

int gmain(char * argv[], int argc)
{
	Window w = glibCreateWindow((char *)"Hello, world!", WINDOW_DEFAULT_POSITION, 0, 0, 0, STYLE_NORMAL, NULL);
	if (w->flags & SYS_FAILED) return 1;

	glibSetWindowEvent(w, shown, EVENT_RESIZE);

	glibShowWindow(w);

	return glibLoop();
}
