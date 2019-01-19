#define USE_GMAIN_ONLY
#include <glib/glib.h>

int gmain(char * argv, int argc)
{
	Window w = glibCreateWindow((char *)"GLib Window", 100, 100, 500, 300, STYLE_NORMAL | STYLE_SHOW_MINIMAZE, NULL);
	if (w == NULL) return 1;
	glibShowWindow(w);

	return 0;
}
