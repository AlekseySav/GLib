#pragma once
#include "stdafx.h"

enum cell : byte
{
	none = 0,
	dino = 1,
	ground = 2,
	cloud = 3,
	cactus = 4
};

typedef cell map[50][4];

EXTERN map main;
EXTERN Point dino_coords;

void load(map * ptr);
void init();

EXTERN bool dinogo;

#define CELL_SIZE 65

EXTERN Image pictures;
void draw_cell(Image im, cell c, Point coords);
Image draw_field();
