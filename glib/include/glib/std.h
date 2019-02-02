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

#undef EVENT_FIRST_ARG
#undef EVENT_SECOND_ARG

#define EVENT_FIRST_ARG(flag) (u_int32)(u_int16)(flag & 0xffff)
#define EVENT_SECOND_ARG(flag) (u_int32)(u_int16)((flag >> 16) & 0xffff)

#define EVENT_MOUSE_X(args) ((int32)(int16)EVENT_FIRST_ARG(args->flag2))
#define EVENT_MOUSE_Y(args) ((int32)(int16)EVENT_SECOND_ARG(args->flag2))

#endif
