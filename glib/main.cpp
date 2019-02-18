#include <glib/glib.h>
#include <glib/std.h>

Bitmap b = glibInitBitmap();
Image image;

void draw(EventArgs * args)
{
	Window w = WINDOW_HANDLE(args->handle);
	Image im = glibCreateImage(w->width, w->height);
	glibFillImage(im, COLOR_BLUE);
	im->DrawImage(image, POINT(100, 100), POINT(200, 200));

	glibDrawImage(im, w, true);
}

void close(EventArgs * args)
{
	RELEASE(image);
	RELEASE(b);
}

int gmain(char * argv[], int argc)
{
	b->LoadFromFile(TEXT("D:\\projects\\glib\\Debug\\1.bmp"));
	image = glibCreateImage(b);

	Window w = glibCreateWindow(TEXT("glib"), 100, 100, 500, 350, STYLE_NORMAL, NULL);
	if WINDOW_FAILED(w) return 1;

	glibSetWindowEvent(w, draw, EVENT_DRAW);
	glibSetWindowEvent(w, close, EVENT_CLOSED);
	glibShowWindow(w);

	return glibLoop();
}
