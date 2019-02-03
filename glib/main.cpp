#include <glib/glib.h>
#include <glib/std.h>

void draw(EventArgs * args)
{
	Window w = WINDOW_HANDLE(args->handle);
	Image image = glibCreateImage(w->width, w->height);
	glibFillImage(image, COLOR_BLACK);
	glibDrawLine(image, POINT(0, 0), POINT(w->width, w->height), COLOR(255, 0, 0, 50), 3L);
	glibDrawLine(image, POINT(w->width, 0), POINT(0, w->height), COLOR(0, 255, 0, 50), 3L);
	glibFillCircle(image, POINT(Mouse.x, Mouse.y), 50, COLOR(255, 255, 255, 150));
	
	glibStartWindowDraw(w);
	glibDrawImage(image, w);
}

int gmain(char * argv[], int argc)
{
	Window w = glibCreateWindow(TEXT("Hello, world!"), 100, 100, 535, 350, STYLE_NORMAL, NULL);
	if WINDOW_FAILED(w) return 1;

	glibSetWindowEvent(w, draw, EVENT_MOUSEMOVE);
	glibShowWindow(w);

	return glibLoop();
}
