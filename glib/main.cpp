#include <glib/glib.h>
#include <glib/std.h>

Image im;
Window w;

#define abs(a) (((a) >= (0)) ? (a) : (-a))

#define SPEED 500000

#define CELL_SIZE 50
#define WIDTH 10
#define HEIGHT 10

enum cell {
	NONE = 0,
	SNAKE = 1,
	FOOD = 2
};

struct SnakeCell {
	Point cell;
	void * next;
};

SnakeCell * tail;
int direction = KEY_RIGHT_ARROW;
Point food;

bool findsnake(Point p)
{
	SnakeCell * s = tail;

	while (s->next != NULL)
		if (s->cell.x == p.x && s->cell.y == p.y) return true;
		else s = (SnakeCell *)s->next;
	return false;
}
void drawcell(Point p, cell c)
{
	if (c == NONE)
		glibDrawRectangle(im, RECT(POINT(p.x * CELL_SIZE, p.y * CELL_SIZE), POINT((p.x + 1) * CELL_SIZE, (p.y + 1) * CELL_SIZE)), COLOR_BLUE, COLOR_WHITE, 1L);
	else if(c == FOOD)
		glibDrawRectangle(im, RECT(POINT(p.x * CELL_SIZE, p.y * CELL_SIZE), POINT((p.x + 1) * CELL_SIZE, (p.y + 1) * CELL_SIZE)), COLOR_YELLOW, COLOR_WHITE, 1L);
	else
	glibDrawRectangle(im, RECT(POINT(p.x * CELL_SIZE, p.y * CELL_SIZE), POINT((p.x + 1) * CELL_SIZE, (p.y + 1) * CELL_SIZE)), COLOR_PURPLE, COLOR_TRANSPARENT, 0L);
}
void addfood()
{
	int pos = rand() % (WIDTH & HEIGHT);
	Point p = POINT(pos / WIDTH, pos % WIDTH);

		food = p;
		drawcell(p, FOOD);
}

void show(EventArgs * args)
{
	im = glibCreateImage(w->width, w->height);
	glibFillImage(im, COLOR_WHITE);
	
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			drawcell(POINT(x, y), NONE);
	tail = (SnakeCell *)malloc(3 * sizeof(SnakeCell));
	tail[0].cell = POINT(0, 9); tail[0].next = &tail[1];
	tail[1].cell = POINT(1, 9); tail[1].next = &tail[2];
	tail[2].cell = POINT(2, 9); tail[2].next = NULL;

	drawcell(tail[0].cell, SNAKE);
	drawcell(tail[1].cell, SNAKE);
	drawcell(tail[2].cell, SNAKE);

	addfood();
}
void update(EventArgs * args)
{
	if (args->msg == EVENT_DRAW)
	{
		glibDrawImageNoRelease(im, w);
	}

	static int time = 0;
	if (time == SPEED || args->msg == EVENT_KEYDOWN)
	{
		if (direction == KEY_SPACE) return;

		time = 0;
		SnakeCell pcell = *tail;

		SnakeCell * ptr = tail;
		drawcell(tail->cell, NONE);
		
		while (ptr->next != NULL)
		{
			ptr->cell = ((SnakeCell *)ptr->next)->cell;
			ptr = ((SnakeCell *)ptr->next);
		}

		switch (direction)
		{
		case KEY_UP_ARROW:
			ptr->cell.y--;
			if (ptr->cell.y < 0) ptr->cell.y = HEIGHT - 1;
			break;
		case KEY_DOWN_ARROW:
			ptr->cell.y++;
			if (ptr->cell.y >= HEIGHT) ptr->cell.y = 0;
			break;
		case KEY_LEFT_ARROW:
			ptr->cell.x--;
			if (ptr->cell.x < 0) ptr->cell.x = WIDTH - 1;
			break;
		case KEY_RIGHT_ARROW:
			ptr->cell.x++;
			if (ptr->cell.x >= WIDTH) ptr->cell.x = 0;
			break;
		}

		drawcell(ptr->cell, SNAKE);

		if (food.x == ptr->cell.x && food.y == ptr->cell.y)
		{
			ptr = (SnakeCell *)malloc(sizeof(SnakeCell));
			ptr->next = tail;
			ptr->cell = pcell.cell;
			tail = ptr;
			addfood();
		}
		
		glibStartWindowDraw(w);
		glibDrawImageNoRelease(im, w);
	}
	else time++;
}
void key(EventArgs * args)
{
	if (args->flag1 < KEY_LEFT_ARROW || args->flag1 > KEY_DOWN_ARROW && args->flag1 != KEY_SPACE) return;

	if (direction != args->flag1) 
	{
		if (abs((int32)(args->flag1 - direction)) == 2) return;
		direction = args->flag1;
		if (direction != KEY_SPACE)
			update(args);
	}
}

int gmain(char * argv[], int argc)
{
	w = glibCreateWindow(TEXT("Snake"), 100, 100, CELL_SIZE * WIDTH + 1, CELL_SIZE * HEIGHT + 1, STYLE_NORMAL ^ STYLE_RESIZABLE, NULL);

	glibSetWindowEvent(w, show, EVENT_SHOWN);
	glibSetWindowEvent(w, update, EVENT_DRAW | EVENT_BASIC);
	glibSetWindowEvent(w, key, EVENT_KEYDOWN);

	glibShowWindow(w);
	
	int res = glibLoop();
	glibReleaseImage(im);
	free(tail);
	return res;
}
