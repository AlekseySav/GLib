#include <glib/glib.h>
#include <glib/std.h>

void draw(EventArgs * args)
{
	//if (args->flag1 == EVENT_MOUSE_LEFTBUTTON) {
		Window w = (Window)args->handle;
		Image image = glibCreateImage(w->width, w->height);
		glibFillImage(image, COLOR_BLUE);

		glibFillCircle(image, POINT(EVENT_MOUSE_X(args), EVENT_MOUSE_Y(args)), 100, COLOR_PURPLE);

		glibStartWindowDraw(w);

		glibDrawImageNoRelease(image, w);
		glibReleaseImage(image);
	//}
}

int gmain(char * argv[], int argc)
{
	Window w = glibCreateWindow((char *)"glib works", 100, 100, 535, 350, STYLE_NORMAL, NULL);
	glibShowWindow(w);
	glibSetWindowEvent(w, draw, EVENT_MOUSEMOVE);

	return glibLoop();
}
