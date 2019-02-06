#include <glib/glib.h>
#include <glib/std.h>

void draw(EventArgs * args)
{
	Window w = WINDOW_HANDLE(args->handle);

	Image image = glibCreateImage(w->width, w->height);
	glibFillImage(image, COLOR_BLUE);
	glibDrawEllipse(image, ELLIPSE(POINT(100, 70), 100, 70), COLOR_RED, COLOR_WHITE, 10L);

	glibStartWindowDraw(w);
	glibDrawImage(image, w);
}

int gmain(char * argv[], int argc)
{
	Window w = glibCreateWindow(TEXT("hello world"), 100, 100, 535, 350, STYLE_NORMAL, NULL);
	glibShowWindow(w);
	glibSetWindowEvent(w, draw, EVENT_DRAW);

	return glibLoop();
}
