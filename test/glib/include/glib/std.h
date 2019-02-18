#pragma once

#if defined(_GLIB) && !defined(_GLIB_STD)
#define _GLIB_STD

#if !defined(__GLIB_SRC)

#ifndef POINT
	#define POINT(x, y) (Point { x, y })
#endif

#ifndef COLOR
	#define COLOR(r, g, b, a) (ARGB { a, r, g, b })
#endif

#ifndef LINE
	#define LINE(point1, point2) (Line { point1, point2 })
#endif

#ifndef RECT
	#define RECT(point1, point2) (Rect { point1, point2 })
#endif

#ifndef ELLIPSE
	#define ELLIPSE(center, radius_x, radius_y) (Ellipse { center, radius_x, radius_y })
#endif

#ifndef CIRCLE
	#define CIRCLE(center, radius) ELLIPSE(center, radius, radius)
#endif

#define COLOR_BLACK COLOR(0, 0, 0, 255)
#define COLOR_WHITE COLOR(255, 255, 255, 255)
#define COLOR_RED COLOR(255, 0, 0, 255)
#define COLOR_GREEN COLOR(0, 255, 0, 255)
#define COLOR_BLUE COLOR(0, 0, 255, 255)
#define COLOR_PURPLE COLOR(150, 0, 150, 255)
#define COLOR_YELLOW COLOR(255, 255, 0, 255)
#define COLOR_TRANSPARENT COLOR(0, 0, 0, 0)

#endif

#ifndef TEXT
	#ifdef __cplusplus
		#define TEXT(_const_char_ptr_) ((char *)((void *)((const char *)(_const_char_ptr_))))
	#else
		#define TEXT(_const_char_ptr_) ((char *)(_const_char_ptr_))
	#endif
#endif

#ifndef WINDOW
	#ifdef __cplusplus
		#define WINDOW(handle) ((Window)((Window__ *)((void *)((Handle)(handle)))))
	#else
		#define WINDOW(handle) ((struct Window__ *)((Handle)(handle)))
	#endif
#endif

#ifndef FLAG_INCLUDES
	#define FLAG_INCLUDES(flag, value) ((bool)((u_int32)(flag & value)))
#endif

#ifndef LITTLE_WORD
	#define LITTLE_WORD(_dword) (u_int32)(u_int16)(_dword & 0xffff)
#endif

#ifndef BIG_WORD
	#define BIG_WORD(_dword) (u_int32)(u_int16)((_dword >> 16) & 0xffff)
#endif

#ifndef EVENT_X_POSITION
	#define EVENT_X_POSITION(args) ((u_int32)((int32)(int16)LITTLE_WORD(args->flag1)))
#endif

#ifndef EVENT_Y_POSITION
	#define EVENT_Y_POSITION(args) ((u_int32)((int32)(int16)BIG_WORD(args->flag1)))
#endif

#ifndef WINDOW_FAILED
	#define WINDOW_FAILED(w) (FLAG_INCLUDES(SYS_FAILED, w->flags))
#endif

#undef MAKE_WORD
#undef MAKE_DWORD
#undef FREE_WORD
#undef FREE_DWORD

#define MAKE_WORD(byte1, byte2) ((word)((byte1 & 0xff) | ((byte2 & 0xff) << 8)))
#define MAKE_DWORD(byte1, byte2, byte3, byte4) ((word)((byte1 & 0xff) | ((byte2 & 0xff) << 8) | ((byte3 & 0xff) << 16) | ((byte4 & 0xff) << 24)))
#define FREE_WORD(_word, byte_ptr) do { *byte_ptr = (byte)(_word & 0xff); *(byte_ptr + 1) = (byte)((_word >> 8) & 0xff); } while(0)
#define FREE_DWORD(_dword, byte_ptr) do { *byte_ptr = (byte)(_dword & 0xff); *(byte_ptr + 1) = (byte)((_dword >> 8) & 0xff); \
								*(byte_ptr + 2) = (byte)((_dword >> 16) & 0xff); *(byte_ptr + 3) = (byte)((_dword >> 24) & 0xff);} while(0)

#ifndef RELEASE
	#define RELEASE(ptr) do { if(ptr != NULL) { ptr->Release(); ptr = NULL; } } while(0)
#endif

#ifndef __GLIB_SRC
	typedef struct Line Line;
	typedef struct Rect Rect;
	typedef struct Ellipse Ellipse;
	typedef struct Triangle Triangle;
#endif

#define MOUSE_LEFTBUTTON		0x0001
#define MOUSE_RIGHTBUTTON 		0x0002
#define MOUSE_MIDDLEBUTTON 		0x0003
#define MOUSE_X1BUTTON 			0x0004
#define MOUSE_X2BUTTON 			0x0005

#define KEY_BACKSPACE			0x0008
#define KEY_TAB					0x0009
#define KEY_CLEAR				0x000c
#define KEY_RETURN				0x000d

#define KEY_SHIFT				0x0010
#define KEY_CTRL				0x0011
#define KEY_ALT					0x0012
#define KEY_PAUSE				0x0013
#define KEY_CAPSLOCK			0x0014

#define KEY_KANA				0x0015
#define KEY_JUNJA				0x0017
#define KEY_FINAL				0x0018
#define KEY_HANJA				0x0019

#define KEY_ESCAPE				0x0018
#define KEY_CONVERT				0x001c
#define KEY_NOCONVERT			0x001d
#define KEY_ACCEPT				0x001e
#define KEY_MODECHANGE			0x001f
#define KEY_SPACE				0x0020
#define KEY_PRIOR				0x0021
#define KEY_NEXT				0x0022
#define KEY_END					0x0023
#define KEY_HOME				0x0024
#define KEY_LEFT_ARROW			0x0025
#define KEY_UP_ARROW			0x0026
#define KEY_RIGHT_ARROW			0x0027
#define KEY_DOWN_ARROW			0x0028
#define KEY_SELECT				0x0029
#define KEY_PRINT				0x002a

#ifndef KEY_EXECUTE
	#define KEY_EXECUTE				0x002b
#endif

#define KEY_SNAPSHOT			0x002c
#define KEY_INSERT				0x002d
#define KEY_DELETE				0x002d
#define KEY_HELP				0x002f

#define KEY_0					0x0030
#define KEY_1					0x0031
#define KEY_2					0x0032
#define KEY_3					0x0033
#define KEY_4					0x0034
#define KEY_5					0x0035
#define KEY_6					0x0036
#define KEY_7					0x0037
#define KEY_8					0x0038
#define KEY_9					0x0039

#define KEY_A					0x0041
#define KEY_B					0x0042
#define KEY_C					0x0043
#define KEY_D					0x0044
#define KEY_E					0x0045
#define KEY_F					0x0046
#define KEY_G					0x0047
#define KEY_H					0x0048
#define KEY_I					0x0049
#define KEY_J					0x004a
#define KEY_K					0x004b
#define KEY_L					0x004c
#define KEY_M					0x004d
#define KEY_N					0x004e
#define KEY_O					0x004f
#define KEY_P					0x0050
#define KEY_Q					0x0051
#define KEY_R					0x0052
#define KEY_S					0x0053
#define KEY_T					0x0054
#define KEY_U					0x0055
#define KEY_V					0x0056
#define KEY_W					0x0057
#define KEY_X					0x0058
#define KEY_Y					0x0059
#define KEY_Z					0x005a

#define KEY_SUPER_LEFT			0x005b
#define KEY_SUPER_RIGHT			0x005c
#define KEY_APPS				0x005d

#define KEY_SLEEP				0x005f
#define KEY_NUMPAD_0			0x0060
#define KEY_NUMPAD_1			0x0061
#define KEY_NUMPAD_2			0x0062
#define KEY_NUMPAD_3			0x0063
#define KEY_NUMPAD_4			0x0064
#define KEY_NUMPAD_5			0x0065
#define KEY_NUMPAD_6			0x0066
#define KEY_NUMPAD_7			0x0067
#define KEY_NUMPAD_8			0x0068
#define KEY_NUMPAD_9			0x0069

#define KEY_MULTIPLY			0x006a
#define KEY_ADD					0x006b
#define KEY_SEPARATOR			0x006c
#define KEY_SUBTRACT			0x006d
#define KEY_DECIMAL				0x006e
#define KEY_DIVIDE				0x006f

#define KEY_F1					0x0070
#define KEY_F2					0x0071
#define KEY_F3					0x0072
#define KEY_F4					0x0073
#define KEY_F5					0x0074
#define KEY_F6					0x0075
#define KEY_F7					0x0076
#define KEY_F8					0x0077
#define KEY_F9					0x0078
#define KEY_F10					0x0079
#define KEY_F11					0x007a
#define KEY_F12					0x007b
#define KEY_F13					0x007c
#define KEY_F14					0x007d
#define KEY_F15					0x007e
#define KEY_F16					0x007f
#define KEY_F17					0x0080
#define KEY_F18					0x0081
#define KEY_F19					0x0082
#define KEY_F20					0x0083
#define KEY_F21					0x0084
#define KEY_F22					0x0085
#define KEY_F23					0x0086
#define KEY_F24					0x0087

#define KEY_NUMLOCK				0x0090
#define KEY_SCROLL				0x0091

#define KEY_SHIFT_LEFT			0x00a0
#define KEY_SHIFT_RIGHT			0x00a1
#define KEY_CONTROL_LEFT		0x00a2
#define KEY_CONTROL_RIGHT		0x00a3
#define KEY_MENU_LEFT			0x00a4
#define KEY_MENU_RIGHT			0x00a5

#define KEY_BROWSER_BACK		0x00a6
#define KEY_BROWSER_FOWARD		0x00a7
#define KEY_BROWSER_REFRESH		0x00a8
#define KEY_BROWSER_STOP		0x00a9
#define KEY_BROWSER_SEARCH		0x00aa
#define KEY_BROWSER_FAVOR		0x00ab
#define KEY_BROWSER_HOME		0x00ac
#define KEY_VOLUME_MUTE			0x00ad
#define KEY_VOLUME_DOWN			0x00ae
#define KEY_VOLUME_UP			0x00af
#define KEY_MEDIA_NEXT_TRACK	0x00b0
#define KEY_MEDIA_PREV_TRACK	0x00b1
#define KEY_MEDIA_STOP			0x00b2
#define KEY_MEDIA_PLAY_PAUSE	0x00b3

#define KEY_LAUNCH_MAIL			0x00b4
#define KEY_LAUNCH_MEDIA_SELECT	0x00b5
#define KEY_LAUNCH_APP1			0x00b6
#define KEY_LAUNCH_APP2			0x00b7

#define KEY_SEMICOLON			0x00ba
#define KEY_PLUS				0x00bb
#define KEY_COMMA				0x00bc
#define KEY_MINUS				0x00bd
#define KEY_POINT				0x00be
#define KEY_SLASH				0x00bf
#define KEY_WAVE				0x00c0

#define KEY_SQUAREBRACKET_LEFT	0x00db
#define KEY_BACKSLASH			0x00dc
#define KEY_SQUAREBRACKET_RIGHT	0x00dd
#define KEY_INVERTED_COMMAS		0x00de

#define KEY_OEM_102				0x00e2		//клавиша «<>» или «\ | » клавиша на клавиатуре RT 102 - клавишная
#define KEY_PROCESSKEY			0x00e5
#define KEY_PACKET				0x00e7		//используется для передачи символов Юникода, как если бы они были нажатиями клавиш.
											//Ключ VK_PACKET - это младшее слово 32 - битного значения виртуального ключа, используемого 
											//для не клавиатурных методов ввода.Символ Unicode - это высокое слово.

#define KEY_ATTN				0x00f6
#define KEY_CRSEL				0x00f7
#define KEY_EXSEL				0x00f8
#define KEY_ERASE_EOF			0x00f9
#define KEY_PLAY				0x00fa
#define KEY_ZOOM				0x00fb
#define KEY_PA1					0x00fd
#define KEY_OEM_CLEAR			0x00fe

EXTERN Point Mouse;

#endif
