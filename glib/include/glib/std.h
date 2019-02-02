#pragma once

#ifdef _GLIB

#undef POINT
#undef COLOR

#define POINT glibCreatePoint
#define COLOR glibCreateRGBA
#define TEXT(_const_char_ptr_) ((char *)_const_char_ptr_)

#ifndef __cplusplus
	#define COLOR_BLACK COLOR(0, 0, 0, 255)
	#define COLOR_WHITE COLOR(255, 255, 255, 255)
	#define COLOR_RED COLOR(255, 0, 0, 255)
	#define COLOR_GREEN COLOR(0, 255, 0, 255)
	#define COLOR_BLUE COLOR(0, 0, 255, 255)
	#define COLOR_PURPLE COLOR(150, 0, 150, 255)
	#define COLOR_YELLOW COLOR(255, 255, 0, 255);
#else
	const RGBA COLOR_BLACK = COLOR(0, 0, 0, 255);
	const RGBA COLOR_WHITE = COLOR(255, 255, 255, 255);
	const RGBA COLOR_RED = COLOR(255, 0, 0, 255);
	const RGBA COLOR_GREEN = COLOR(0, 255, 0, 255);
	const RGBA COLOR_BLUE = COLOR(0, 0, 255, 255);
	const RGBA COLOR_PURPLE = COLOR(150, 0, 150, 255);
	const RGBA COLOR_YELLOW = COLOR(255, 255, 0, 255);
#endif

#undef EVENT_FIRST_ARG
#undef EVENT_SECOND_ARG

#define EVENT_FIRST_ARG(flag) (u_int32)(u_int16)(flag & 0xffff)
#define EVENT_SECOND_ARG(flag) (u_int32)(u_int16)((flag >> 16) & 0xffff)

#define EVENT_MOUSE_X(args) ((int32)(int16)EVENT_FIRST_ARG(args->flag1))
#define EVENT_MOUSE_Y(args) ((int32)(int16)EVENT_SECOND_ARG(args->flag1))


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
#define KEY_EXECUTE				0x002b
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

#define KEY_OEM_1				0x00ba		//для стандартной клавиатуры США клавиша ';:'
#define KEY_OEM_PLUS			0x00bb		//клавиша « + »
#define KEY_OEM_COMMA			0x00bc		//клавиша ','
#define KEY_OEM_MINUS			0x00bd		//клавиша « - »
#define KEY_OEM_PERIOD			0x00be		// «.» ключ
#define KEY_OEM_2				0x00bf		//для стандартной клавиатуры США, « / ? » ключ
#define KEY_OEM_3				0x00c0		//для стандартной клавиатуры США клавиша «~»

#define KEY_OEM_4				0x00db		//для стандартной клавиатуры США клавиша '[{'
#define KEY_OEM_5				0x00dc		//для стандартной клавиатуры США '\ |' ключ
#define KEY_OEM_6				0x00dd 		//для стандартной клавиатуры США клавиша ']}'
#define KEY_OEM_7				0x00de		//для стандартной клавиатуры США клавиша «одинарные кавычки / двойные кавычки»
#define KEY_OEM_8				0x00df		//NULL

#define KEY_OEM_102				0x00e2		//клавиша «<>» или «\ | » клавиша на клавиатуре RT 102 - клавишная
#define KEY_PROCESSKEY			0x00e5		//клавиша ПРОЦЕСС I
#define KEY_PACKET				0x00e7		//используется для передачи символов Юникода, как если бы они были нажатиями клавиш.Ключ VK_PACKET - это младшее слово 32 - битного значения виртуального ключа, используемого для не клавиатурных методов ввода.Символ Unicode - это высокое слово.

#define KEY_ATTN				0x00f6		//Ключ Attn
#define KEY_CRSEL				0x00f7		//Ключ CrSel
#define KEY_EXSEL				0x00f8		//Ключ ExSel
#define KEY_EREOF				0x00f9		//Стереть EOF ключ
#define KEY_PLAY				0x00fa		//Клавиша воспроизведения
#define KEY_ZOOM				0x00fb		//Кнопка масштабирования
#define KEY_PA1					0x00fd		//Ключ PA1
#define KEY_OEM_CLEAR			0x00fe		//Очистить ключ

#endif
