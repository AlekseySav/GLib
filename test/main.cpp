#include "game.h"

Image im;

int speed = 10000;

float fall = 0.2f;
float a = 0;

void draw(EventArgs * args)
{
	Window w = WINDOW(args->handle);

	if (args->msg == EVENT_REDRAW)
		glibDrawImage(im, w, POINT(dino_coords.x - 130, 0), POINT(im->width, im->height));

	static int i = 0;

	if (i == speed) 
	{
		i = 0;

		draw_cell(im, none, dino_coords);

		dino_coords.x += 2;

		if (dino_coords.y + (int)a < 65 * 2 || a <= 0)
		{
			dino_coords.y += (int)a;
			a += fall;
		}
		else a = 0;

		draw_cell(im, dino, dino_coords);

		glibStartWindowDraw(w);
		glibDrawImage(im, w, POINT(dino_coords.x - 130, 0), POINT(im->width, im->height));
	}
	else i++;
}

void close(EventArgs * args)
{
	RELEASE(im);
}

void key(EventArgs * args)
{
	if (args->flag == KEY_SPACE) a = -8;
}

int gmain(char * argv[], int argc)
{
	init();
	im = draw_field();

	Window w = glibCreateWindow(TEXT("like a chrome dino"), 0, 100, 650, 65 * 4, STYLE_NORMAL, NULL);
	if WINDOW_FAILED(w) return 1;

	glibSetWindowEvent(w, draw, EVENT_DRAWING);
	glibSetWindowEvent(w, close, EVENT_CLOSED);
	glibSetWindowEvent(w, key, EVENT_KEYDOWN);

	glibShowWindow(w);
	return glibLoop();
}
