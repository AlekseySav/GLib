#define USE_GMAIN_ONLY
#include <glib/glib.h>

int gmain(char * argv, int argc)
{
	Window w = glibCreateWindow((char *)"GLib Window", 100, 100, 500, 300, STYLE_NORMAL, NULL);
	
	if (w == NULL) return 1;
	glibShowWindow(w);

//	glibLogEvent(w, EVENT_WINDOW_SHOWN, (int)Event_WindowClosed1);
	return glibLoop();
}
