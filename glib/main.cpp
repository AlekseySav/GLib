#include <glib/glib.h>
#include <glib/std.h>

u_int h = 100;
long speed = 5;

void draw(EventArgs * args)
{
	Window w = (Window)args->handle;

	Image image = glibCreateImage(w->width, w->height);
	glibFillImage(image, RGB_BLUE);
	glibFillCircle(image, POINT(w->width / 2, h), 100, RGB_RED);

	speed += 5;
	h += speed;
	if (h > w->height - 100)
	{
		speed = -speed;
		h = w->height - 100;
	}

	glibDrawImage(image, w);
}

int gmain(char * argv[], int argc)
{
	Window w = glibCreateWindow((char *)"heh", WINDOW_DEFAULT_POSITION, 0, 0, 0, STYLE_NORMAL | SYS_REDRAW, NULL);
	if WINDOW_FAILED(w) return 1;

	glibSetWindowEvent(w, draw, EVENT_DRAW | EVENT_BASIC);
	glibShowWindow(w);
	return glibLoop();
}
