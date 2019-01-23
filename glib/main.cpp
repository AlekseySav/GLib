#include <glib/glib.h>

void redraw(EventArgs * args)
{
	Window w = (Window)args->handle;
	static u_int x = w->width / 2, y = 70;
	static long speed = 10;
	y += 100;
	speed += 10;

	if (y >= w->height - 100)
	{
		y = w->height - 110;
		speed = -speed;
	}
	if (y < 70) y = 70;

	Image im = glibCreateImage(w->width, w->height);
	glibFillImage(im, RGB_BLUE);

	glibFillCircle(im, glibCreatePoint(x, y), 50, RGB_RED);

	glibDrawImage(im, w);
	glibFreeImage(im);
}

int gmain(char * argv[], int argc)
{
	Window w = glibCreateWindow((char *)"Hello, world!", WINDOW_DEFAULT_POSITION, 0, 0, 0, STYLE_NORMAL, NULL);
	if WINDOW_FAILED(w) return 1;

	glibSetWindowEvent(w, redraw, EVENT_DRAW);
	glibShowWindow(w);

	return glibLoop();
}
