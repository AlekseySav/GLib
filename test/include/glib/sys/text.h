#if !defined(_GLIB_SYS_TEXT) && defined(_GLIB)
#define _GLIB_SYS_TEXT

#define EX_DrawText _FUNCTION_HANDLE(DrawText)

#define LANGUAGE_DEFAULT	0x0000	//default ascii symbols
#define LANGUAGE_ENGLISH	0x0001
#define LANGUAGE_RUSSIAN	0x0002

EXTERN void glibSetLanguage(u_int32 lang);

#define glibSetDefaultLanguage() glibSetLanguage(LANGUAGE_DEFAULT)

struct Text {
	char * str;
	ARGB font;
	ARGB back;
};

EXTERN inline Text glibCreateText(char * str, ARGB font = ARGB{ 255, 0, 0, 0 }, ARGB back = ARGB{ 0, 0, 0, 0 });

EXTERN void glibDrawText(Image im, Point pos, Text text);

typedef wchar_t w_char;
EXTERN w_char glibConvertCharacter(char c);
EXTERN void glibConvertString(char * c, w_char * buf);

EXTERN u_int32 glib_language;

EXTERN u_int strlen(char * ptr);
EXTERN u_int strlen(w_char * ptr);

#endif
