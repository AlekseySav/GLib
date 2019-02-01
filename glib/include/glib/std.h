#pragma once

#ifdef _GLIB

#undef POINT
#undef COLOR

#define POINT glibCreatePoint
#define COLOR glibCreateRGBA
#define TEXT(_const_char_ptr_) ((char *)_const_char_ptr_)

#ifndef __cplusplus
	#define RGB_BLACK COLOR(0, 0, 0, 255)
	#define RGB_WHITE COLOR(255, 255, 255, 255)
	#define RGB_RED COLOR(255, 0, 0, 255)
	#define RGB_GREEN COLOR(0, 255, 0, 255)
	#define RGB_BLUE COLOR(0, 0, 255, 255)
	#define RGB_PURPLE COLOR(150, 0, 150, 255)
	#define RGB_YELLOW COLOR(255, 255, 0, 255);
#else
	const RGBA RGB_BLACK = COLOR(0, 0, 0, 255);
	const RGBA RGB_WHITE = COLOR(255, 255, 255, 255);
	const RGBA RGB_RED = COLOR(255, 0, 0, 255);
	const RGBA RGB_GREEN = COLOR(0, 255, 0, 255);
	const RGBA RGB_BLUE = COLOR(0, 0, 255, 255);
	const RGBA RGB_PURPLE = COLOR(150, 0, 150, 255);
	const RGBA RGB_YELLOW = COLOR(255, 255, 0, 255);
#endif

#endif
