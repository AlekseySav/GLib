#include "game.h"

map main;
Image pictures;
bool dinogo = false;
Point dino_coords;

void load(map * ptr)
{
	byte arr[4][50];
	byte new_line;

	FILE * f = fopen("res\\map.txt", "rt");
	fread(arr[0], 1, 50, f); fread(&new_line, 1, 1, f);
	fread(arr[1], 1, 50, f); fread(&new_line, 1, 1, f);
	fread(arr[2], 1, 50, f); fread(&new_line, 1, 1, f);
	fread(arr[3], 1, 50, f); fread(&new_line, 1, 1, f);
	fclose(f);

	for(int y = 0; y < 4; y++)
		for(int x = 0; x < 50; x++)
			switch (arr[y][x])
			{
			case 'A':
				(*ptr)[x][y] = dino;
				dino_coords = POINT(x * CELL_SIZE, y * CELL_SIZE);
				break;
			case '#':
				(*ptr)[x][y] = ground;
				break;
			case '*':
				(*ptr)[x][y] = cloud;
				break;
			case 'T':
				(*ptr)[x][y] = cactus;
				break;
			default:
				(*ptr)[x][y] = none;
				break;
			}
}

void init()
{
	load(&main);

	Bitmap b = glibBitmap();
	b->LoadFromFile(TEXT("res\\pictures.bmp"));
	pictures = glibCreateImage(b);
	RELEASE(b);
}

void draw_cell(Image im, cell c, Point start)
{
	Point end = POINT(start.x + CELL_SIZE, start.y + CELL_SIZE);

	switch (c)
	{
	case none:
		im->DrawImage(pictures, start, end, POINT(0, 0));
		break;
	case dino:
		if (dinogo) im->DrawImage(pictures, start, end, POINT(2 * CELL_SIZE, 0));
		else im->DrawImage(pictures, start, end, POINT(CELL_SIZE, 0));
		break;
	case ground:
		im->DrawImage(pictures, start, end, POINT(4 * CELL_SIZE, 0));
		break;
	case cloud:
		im->DrawImage(pictures, start, end, POINT(3 * CELL_SIZE, 0));
		break;
	case cactus:
		im->DrawImage(pictures, start, end, POINT(5 * CELL_SIZE, 0));
		break;
	default:
		glibFillImage(im, COLOR_BLUE);
		break;
	}
}

Image draw_field()
{
	Image im = glibCreateImage(50 * CELL_SIZE, 4 * CELL_SIZE);

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 50; x++)
			draw_cell(im, main[x][y], POINT(x * CELL_SIZE, y * CELL_SIZE));

	return im;
}
