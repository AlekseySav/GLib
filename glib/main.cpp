#include <glib/glib.h>
#include <glib/std.h>

Bitmap b = glibBitmap();
Image image;

void draw(EventArgs * args)
{
	Window w = WINDOW_HANDLE(args->handle);
	glibDrawImage(image, w);
}

void close(EventArgs * args)
{
	RELEASE(image); b->ptr = NULL;
	RELEASE(b);
}

int gmain(char * argv[], int argc)
{
	image = glibCreateImage(535, 350);
	glibFillImage(image, COLOR_RED);
	image->DrawEllipse(ELLIPSE(POINT(100, 100), 100, 100), COLOR(0, 0, 255, 150), 2L);
	b->CreateBitmap(image->image, 535, 350);
	b->Write(TEXT("1.bmp"));

	Window w = glibCreateWindow(TEXT("glib"), 100, 100, 535, 350, STYLE_NORMAL, NULL);
	if WINDOW_FAILED(w) return 1;

	glibSetWindowEvent(w, draw, EVENT_DRAW);
	glibSetWindowEvent(w, close, EVENT_CLOSED);
	glibShowWindow(w);

	return glibLoop();
}
