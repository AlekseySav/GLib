#include <glib/glib.h>
#include <glib/std.h>

Image im;
Window w;

//Tictactoe

u_int player = 0;
char field[3][3];

void start()
{
	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
			field[x][y] = ' ';
	im = glibCreateImage(300, 300);
}

void drawcross(Point p)
{
	Point pmin = POINT(p.x * 100 + 15, p.y * 100 + 15);
	Point pmax = POINT(p.x * 100 + 85, p.y * 100 + 85);

	glibDrawLine(im, LINE(pmin, pmax), COLOR_WHITE, 2L);
	glibDrawLine(im, LINE(POINT(pmax.x, pmin.y), POINT(pmin.x, pmax.y)), COLOR_WHITE, 2L);

	glibStartWindowDraw(w);
	glibDrawImageNoRelease(im, w);
}
void drawcircle(Point p)
{
	Point center = POINT(p.x * 100 + 50, p.y * 100 + 50);
	glibDrawEllipse(im, CIRCLE(center, 40), COLOR_TRANSPARENT, COLOR_WHITE, 2L);

	glibStartWindowDraw(w);
	glibDrawImageNoRelease(im, w);
}
void drawfield()
{
	glibFillImage(im, COLOR_BLUE);

	glibDrawLine(im, LINE(POINT(100, 0), POINT(100, 300)), COLOR_WHITE, 2L);
	glibDrawLine(im, LINE(POINT(200, 0), POINT(200, 300)), COLOR_WHITE, 2L);

	glibDrawLine(im, LINE(POINT(0, 100), POINT(300, 100)), COLOR_WHITE, 2L);
	glibDrawLine(im, LINE(POINT(0, 200), POINT(300, 200)), COLOR_WHITE, 2L);

	glibStartWindowDraw(w);
	glibDrawImageNoRelease(im, w);
}

void game(EventArgs * args)
{
	if (args->msg == EVENT_DRAW) glibDrawImageNoRelease(im, w);
	else
	{
		Point p = POINT(Mouse.x / 100, Mouse.y / 100);
		if (field[p.x][p.y] == ' ') {
			if (player % 2 == 0) {
				drawcross(p);
				field[p.x][p.y] = 'x';
			}
			else {
				drawcircle(p);
				field[p.x][p.y] = 'o';
			}
			player++;
		}
	}
}

int gmain(char * argv[], int argc)
{
	w = glibCreateWindow(TEXT("tictactoe"), 100, 100, 300, 300, STYLE_NORMAL ^ STYLE_RESIZABLE, NULL);
	glibShowWindow(w);
	glibSetWindowEvent(w, game, EVENT_CLICK | EVENT_DRAW);

	start();
	drawfield();
	glibDrawTriangle(im, TRIANGLE(POINT(0, 0), POINT(100, 100), POINT(200, 100)), COLOR(200, 255, 0, 0));

	int res = glibLoop();
	glibReleaseImage(im);
	return res;
}
