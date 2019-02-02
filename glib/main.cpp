#include <glib/glib.h>
#include <glib/std.h>

void draw(EventArgs * args)
{
	static long a = 1;
	static u_int x = 0, y = 100;

	Window w = WINDOW_HANDLE(args->handle);
	x = w->width / 2;
	Image image = glibCreateImage(w->width, w->height);

	glibFillImage(image, COLOR_BLUE);
	glibFillCircle(image, POINT(x, y), 100, COLOR_PURPLE);

	a += 1;
	y += a;
	if (y >= w->height - 100)
	{
		y = w->height - 100;
		a = -a;
	}

	if(!FLAG_INCLUDES(args->msg, EVENT_DRAW)) glibStartWindowDraw(w);

	glibDrawImageNoRelease(image, w);
	glibReleaseImage(image);
}

int gmain(char * argv[], int argc)
{
	Window w = glibCreateWindow(TEXT("Ball falled))"), 100, 100, 535, 350, STYLE_NORMAL, NULL);

	glibSetWindowEvent(w, draw, EVENT_DRAW | EVENT_BASIC);
	glibShowWindow(w);

	return glibLoop();
}
