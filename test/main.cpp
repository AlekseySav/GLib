#include <glib/glib.h>
#include <glib/std.h>

void draw(EventArgs * args)
{

	Window w = WINDOW_HANDLE(args->handle);
	Image im = glibCreateImage(w->width, w->height);
	glibFillImage(im, COLOR_BLUE);
	im->DrawLine(LINE(POINT(0, 0), POINT(100, 100)), COLOR_RED, 2L);

	glibDrawText(im, POINT(0, 0), TEXT("text"), 0, COLOR_YELLOW, COLOR_PURPLE);
	glibDrawImage(im, w, true);
}

int gmain(char * argv[], int argc) 
{
	glibSetLanguage(LANGUAGE_RUSSIAN);
	Window w = glibCreateWindow(TEXT("Спирограф"), 100, 100, 535, 350, STYLE_NORMAL, NULL);
	if WINDOW_FAILED(w) return 1;

	glibSetWindowEvent(w, draw, EVENT_DRAW);

	glibShowWindow(w);
	return glibLoop();
}
