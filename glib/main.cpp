#include <glib/glib.h>
#include <glib/std.h>

int gmain(char * argv[], int argc)
{
	glibSetLanguage(LANGUAGE_RUSSIAN);

	Window 	w = glibCreateWindow(TEXT("Дима - пидор"), 100, 100, 535, 350, STYLE_NORMAL, NULL);

	glibShowWindow(w);
	
	return glibLoop();
}
